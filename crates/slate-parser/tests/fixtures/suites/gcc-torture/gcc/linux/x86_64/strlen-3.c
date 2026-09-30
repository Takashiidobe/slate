/* PR tree-optimization/86532 - Wrong code due to a wrong strlen folding
   starting with r262522
   Exercise strlen() with a multi-dimensional array of strings with
   embedded nuls.  */

extern __SIZE_TYPE__ strlen(const char *);

static const char a[2][3][9] = {{"1", "1\0002"}, {"12\0003", "123\0004"}};

volatile int v0 = 0;
volatile int v1 = 1;
volatile int v2 = 2;
volatile int v3 = 3;
volatile int v4 = 4;
volatile int v5 = 5;
volatile int v6 = 6;
volatile int v7 = 7;

#define A(expr)                                                                \
  ((expr) ? (void)0                                                            \
          : (__builtin_printf("assertion on line %i: %s\n", __LINE__, #expr),  \
             __builtin_abort()))

void test_array_ref(void) {
  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;
  int i3 = i2 + 1;
  int i4 = i3 + 1;
  int i5 = i4 + 1;
  int i6 = i5 + 1;
  int i7 = i6 + 1;

  A(strlen(a[0][0]) == 1);
  A(strlen(a[0][1]) == 1);

  A(strlen(a[1][0]) == 2);
  A(strlen(a[1][1]) == 3);

  A(strlen(&a[0][0][0]) == 1);
  A(strlen(&a[0][1][0]) == 1);

  A(strlen(&a[1][0][0]) == 2);
  A(strlen(&a[1][1][0]) == 3);

  A(strlen(&a[0][0][0] + 1) == 0);
  A(strlen(&a[0][1][0] + 1) == 0);
  A(strlen(&a[0][1][0] + 2) == 1);
  A(strlen(&a[0][1][0] + 3) == 0);
  A(strlen(&a[0][1][0] + 7) == 0);

  A(strlen(&a[1][0][0] + 1) == 1);
  A(strlen(&a[1][1][0] + 1) == 2);
  A(strlen(&a[1][1][0] + 2) == 1);
  A(strlen(&a[1][1][0] + 7) == 0);

  A(strlen(a[i0][i0]) == 1);
  A(strlen(a[i0][i1]) == 1);

  A(strlen(a[i1][i0]) == 2);
  A(strlen(a[i1][i1]) == 3);

  A(strlen(&a[i0][i0][i0]) == 1);
  A(strlen(&a[i0][i1][i0]) == 1);
  A(strlen(&a[i0][i1][i1]) == 0);
  A(strlen(&a[i0][i1][i2]) == 1);
  A(strlen(&a[i0][i1][i3]) == 0);
  A(strlen(&a[i0][i1][i3]) == 0);

  A(strlen(&a[i1][i0][i0]) == 2);
  A(strlen(&a[i1][i1][i0]) == 3);
  A(strlen(&a[i1][i1][i1]) == 2);
  A(strlen(&a[i1][i1][i2]) == 1);
  A(strlen(&a[i1][i1][i3]) == 0);
  A(strlen(&a[i1][i1][i4]) == 1);
  A(strlen(&a[i1][i1][i5]) == 0);
  A(strlen(&a[i1][i1][i6]) == 0);
  A(strlen(&a[i1][i1][i7]) == 0);

  A(strlen(&a[i0][i0][i0] + i1) == 0);
  A(strlen(&a[i0][i1][i0] + i1) == 0);
  A(strlen(&a[i0][i1][i0] + i7) == 0);

  A(strlen(&a[i1][i0][i0] + i1) == 1);
  A(strlen(&a[i1][i1][i0] + i1) == 2);
  A(strlen(&a[i1][i1][i0] + i2) == 1);
  A(strlen(&a[i1][i1][i0] + i3) == 0);
  A(strlen(&a[i1][i1][i0] + i4) == 1);
  A(strlen(&a[i1][i1][i0] + i5) == 0);
  A(strlen(&a[i1][i1][i0] + i6) == 0);
  A(strlen(&a[i1][i1][i0] + i7) == 0);

  A(strlen(a[i0][i0]) == 1);
  A(strlen(a[i0][i1]) == 1);

  A(strlen(a[i1][i0]) == 2);
  A(strlen(a[i1][i1]) == 3);

  A(strlen(&a[i0][i0][i0]) == 1);
  A(strlen(&a[i0][i1][i0]) == 1);

  A(strlen(&a[i1][i0][i0]) == 2);
  A(strlen(&a[i1][i1][i0]) == 3);

  A(strlen(&a[i0][i0][i0] + v1) == 0);
  A(strlen(&a[i0][i0][i0] + v2) == 0);
  A(strlen(&a[i0][i0][i0] + v7) == 0);

  A(strlen(&a[i0][i1][i0] + v1) == 0);
  A(strlen(&a[i0][i1][i0] + v2) == 1);
  A(strlen(&a[i0][i1][i0] + v3) == 0);

  A(strlen(&a[i1][i0][i0] + v1) == 1);
  A(strlen(&a[i1][i1][i0] + v1) == 2);
  A(strlen(&a[i1][i1][i0] + v2) == 1);
  A(strlen(&a[i1][i1][i0] + v3) == 0);
  A(strlen(&a[i1][i1][i0] + v4) == 1);
  A(strlen(&a[i1][i1][i0] + v5) == 0);
  A(strlen(&a[i1][i1][i0] + v6) == 0);
  A(strlen(&a[i1][i1][i0] + v7) == 0);
}

int main(void) { test_array_ref(); }


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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<array<array<i8, 9>, 3>, 2> [storage=static] [const] [align=16] = aggregate<array<array<array<i8, 9>, 3>, 2>, zero_fill=false>(index0 = aggregate<array<array<i8, 9>, 3>, zero_fill=true>(index0 = code_units<array<i8, 9>>([49, 0, 0, 0, 0, 0, 0, 0, 0]), index1 = code_units<array<i8, 9>>([49, 0, 50, 0, 0, 0, 0, 0, 0])), index1 = aggregate<array<array<i8, 9>, 3>, zero_fill=true>(index0 = code_units<array<i8, 9>>([49, 50, 0, 51, 0, 0, 0, 0, 0]), index1 = code_units<array<i8, 9>>([49, 50, 51, 0, 52, 0, 0, 0, 0]))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v0:[0-9]+]] v0: volatile i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v1:[0-9]+]] v1: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v2:[0-9]+]] v2: volatile i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v3:[0-9]+]] v3: volatile i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v4:[0-9]+]] v4: volatile i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v5:[0-9]+]] v5: volatile i32 [storage=static] = const<i32>(5) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v6:[0-9]+]] v6: volatile i32 [storage=static] = const<i32>(6) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v7:[0-9]+]] v7: volatile i32 [storage=static] = const<i32>(7) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 48, 93, 91, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 49, 93, 91, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 50, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 50, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_35:[0-9]+]] .str[[VALUE_str_35]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_36:[0-9]+]] .str[[VALUE_str_36]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_37:[0-9]+]] .str[[VALUE_str_37]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_38:[0-9]+]] .str[[VALUE_str_38]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_39:[0-9]+]] .str[[VALUE_str_39]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_40:[0-9]+]] .str[[VALUE_str_40]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_41:[0-9]+]] .str[[VALUE_str_41]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_42:[0-9]+]] .str[[VALUE_str_42]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_43:[0-9]+]] .str[[VALUE_str_43]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_44:[0-9]+]] .str[[VALUE_str_44]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_45:[0-9]+]] .str[[VALUE_str_45]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_46:[0-9]+]] .str[[VALUE_str_46]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_47:[0-9]+]] .str[[VALUE_str_47]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_48:[0-9]+]] .str[[VALUE_str_48]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_49:[0-9]+]] .str[[VALUE_str_49]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_50:[0-9]+]] .str[[VALUE_str_50]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 50, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_51:[0-9]+]] .str[[VALUE_str_51]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_52:[0-9]+]] .str[[VALUE_str_52]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 51, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_53:[0-9]+]] .str[[VALUE_str_53]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_54:[0-9]+]] .str[[VALUE_str_54]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 51, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_55:[0-9]+]] .str[[VALUE_str_55]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_56:[0-9]+]] .str[[VALUE_str_56]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_57:[0-9]+]] .str[[VALUE_str_57]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_58:[0-9]+]] .str[[VALUE_str_58]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_59:[0-9]+]] .str[[VALUE_str_59]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_60:[0-9]+]] .str[[VALUE_str_60]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_61:[0-9]+]] .str[[VALUE_str_61]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_62:[0-9]+]] .str[[VALUE_str_62]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 50, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_63:[0-9]+]] .str[[VALUE_str_63]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_64:[0-9]+]] .str[[VALUE_str_64]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 51, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_65:[0-9]+]] .str[[VALUE_str_65]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_66:[0-9]+]] .str[[VALUE_str_66]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 52, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_67:[0-9]+]] .str[[VALUE_str_67]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_68:[0-9]+]] .str[[VALUE_str_68]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 53, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_69:[0-9]+]] .str[[VALUE_str_69]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_70:[0-9]+]] .str[[VALUE_str_70]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 54, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_71:[0-9]+]] .str[[VALUE_str_71]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_72:[0-9]+]] .str[[VALUE_str_72]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 55, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_73:[0-9]+]] .str[[VALUE_str_73]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_74:[0-9]+]] .str[[VALUE_str_74]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_75:[0-9]+]] .str[[VALUE_str_75]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_76:[0-9]+]] .str[[VALUE_str_76]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_77:[0-9]+]] .str[[VALUE_str_77]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_78:[0-9]+]] .str[[VALUE_str_78]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_79:[0-9]+]] .str[[VALUE_str_79]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_80:[0-9]+]] .str[[VALUE_str_80]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_81:[0-9]+]] .str[[VALUE_str_81]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_82:[0-9]+]] .str[[VALUE_str_82]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_83:[0-9]+]] .str[[VALUE_str_83]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_84:[0-9]+]] .str[[VALUE_str_84]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 50, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_85:[0-9]+]] .str[[VALUE_str_85]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_86:[0-9]+]] .str[[VALUE_str_86]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_87:[0-9]+]] .str[[VALUE_str_87]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_88:[0-9]+]] .str[[VALUE_str_88]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 52, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_89:[0-9]+]] .str[[VALUE_str_89]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_90:[0-9]+]] .str[[VALUE_str_90]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 53, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_91:[0-9]+]] .str[[VALUE_str_91]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_92:[0-9]+]] .str[[VALUE_str_92]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 54, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_93:[0-9]+]] .str[[VALUE_str_93]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_94:[0-9]+]] .str[[VALUE_str_94]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_95:[0-9]+]] .str[[VALUE_str_95]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_96:[0-9]+]] .str[[VALUE_str_96]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_97:[0-9]+]] .str[[VALUE_str_97]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_98:[0-9]+]] .str[[VALUE_str_98]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_99:[0-9]+]] .str[[VALUE_str_99]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_100:[0-9]+]] .str[[VALUE_str_100]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_101:[0-9]+]] .str[[VALUE_str_101]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_102:[0-9]+]] .str[[VALUE_str_102]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_103:[0-9]+]] .str[[VALUE_str_103]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_104:[0-9]+]] .str[[VALUE_str_104]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_105:[0-9]+]] .str[[VALUE_str_105]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_106:[0-9]+]] .str[[VALUE_str_106]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_107:[0-9]+]] .str[[VALUE_str_107]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_108:[0-9]+]] .str[[VALUE_str_108]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_109:[0-9]+]] .str[[VALUE_str_109]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_110:[0-9]+]] .str[[VALUE_str_110]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_111:[0-9]+]] .str[[VALUE_str_111]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_112:[0-9]+]] .str[[VALUE_str_112]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_113:[0-9]+]] .str[[VALUE_str_113]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_114:[0-9]+]] .str[[VALUE_str_114]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_115:[0-9]+]] .str[[VALUE_str_115]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_116:[0-9]+]] .str[[VALUE_str_116]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_117:[0-9]+]] .str[[VALUE_str_117]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_118:[0-9]+]] .str[[VALUE_str_118]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_119:[0-9]+]] .str[[VALUE_str_119]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_120:[0-9]+]] .str[[VALUE_str_120]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_121:[0-9]+]] .str[[VALUE_str_121]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_122:[0-9]+]] .str[[VALUE_str_122]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_123:[0-9]+]] .str[[VALUE_str_123]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_124:[0-9]+]] .str[[VALUE_str_124]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_125:[0-9]+]] .str[[VALUE_str_125]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_126:[0-9]+]] .str[[VALUE_str_126]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_127:[0-9]+]] .str[[VALUE_str_127]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_128:[0-9]+]] .str[[VALUE_str_128]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_129:[0-9]+]] .str[[VALUE_str_129]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_130:[0-9]+]] .str[[VALUE_str_130]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_131:[0-9]+]] .str[[VALUE_str_131]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_132:[0-9]+]] .str[[VALUE_str_132]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 52, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_133:[0-9]+]] .str[[VALUE_str_133]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_134:[0-9]+]] .str[[VALUE_str_134]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 53, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_135:[0-9]+]] .str[[VALUE_str_135]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_136:[0-9]+]] .str[[VALUE_str_136]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 54, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_137:[0-9]+]] .str[[VALUE_str_137]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_138:[0-9]+]] .str[[VALUE_str_138]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_printf:[0-9]+]] @__builtin_printf(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test_array_ref:[0-9]+]] @test_array_ref() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i0:[0-9]+]] i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i1:[0-9]+]] i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i0]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i2:[0-9]+]] i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i1]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i3:[0-9]+]] i3: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i2]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i4:[0-9]+]] i4: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i3]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i5:[0-9]+]] i5: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i4]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i6:[0-9]+]] i6: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i5]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i7:[0-9]+]] i7: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i6]]), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str]])), const<i32>(34), array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_3]])), const<i32>(35), array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str_4]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_5]])), const<i32>(37), array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str_6]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1)))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_7]])), const<i32>(38), array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str_8]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_9]])), const<i32>(40), array_decay<ptr<i8>, length=Some(25)>(%[[VALUE_str_10]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(1)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_11]])), const<i32>(41), array_decay<ptr<i8>, length=Some(25)>(%[[VALUE_str_12]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1)))), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_13]])), const<i32>(43), array_decay<ptr<i8>, length=Some(25)>(%[[VALUE_str_14]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1)))), const<i32>(1)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_15]])), const<i32>(44), array_decay<ptr<i8>, length=Some(25)>(%[[VALUE_str_16]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_17]])), const<i32>(46), array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_18]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(1)))), const<i32>(0)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_19]])), const<i32>(47), array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_20]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(1)))), const<i32>(0)))), const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_21]])), const<i32>(48), array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_22]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(1)))), const<i32>(0)))), const<i32>(3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_23]])), const<i32>(49), array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_24]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(1)))), const<i32>(0)))), const<i32>(7))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_25]])), const<i32>(50), array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_26]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1)))), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_27]])), const<i32>(52), array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_28]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1)))), const<i32>(1)))), const<i32>(0)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_29]])), const<i32>(53), array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_30]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1)))), const<i32>(1)))), const<i32>(0)))), const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_31]])), const<i32>(54), array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_32]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1)))), const<i32>(1)))), const<i32>(0)))), const<i32>(7))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_33]])), const<i32>(55), array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_34]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_35]])), const<i32>(57), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_36]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_37]])), const<i32>(58), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_38]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_39]])), const<i32>(60), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_40]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_41]])), const<i32>(61), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_42]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_43]])), const<i32>(63), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_44]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_45]])), const<i32>(64), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_46]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_47]])), const<i32>(65), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_48]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i2]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_49]])), const<i32>(66), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_50]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_51]])), const<i32>(67), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_52]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_53]])), const<i32>(68), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_54]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_55]])), const<i32>(70), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_56]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_57]])), const<i32>(71), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_58]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_59]])), const<i32>(72), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_60]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i2]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_61]])), const<i32>(73), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_62]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_63]])), const<i32>(74), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_64]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i4]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_65]])), const<i32>(75), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_66]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i5]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_67]])), const<i32>(76), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_68]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i6]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_69]])), const<i32>(77), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_70]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i7]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_71]])), const<i32>(78), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_72]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_73]])), const<i32>(80), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_74]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_75]])), const<i32>(81), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_76]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i7]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_77]])), const<i32>(82), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_78]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_79]])), const<i32>(84), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_80]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_81]])), const<i32>(85), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_82]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_83]])), const<i32>(86), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_84]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i3]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_85]])), const<i32>(87), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_86]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i4]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_87]])), const<i32>(88), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_88]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i5]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_89]])), const<i32>(89), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_90]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i6]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_91]])), const<i32>(90), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_92]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i7]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_93]])), const<i32>(91), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_94]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_95]])), const<i32>(93), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_96]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_97]])), const<i32>(94), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_98]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_99]])), const<i32>(96), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_100]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_101]])), const<i32>(97), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_102]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_103]])), const<i32>(99), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_104]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_105]])), const<i32>(100), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_106]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_107]])), const<i32>(102), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_108]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_109]])), const<i32>(103), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_110]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_111]])), const<i32>(105), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_112]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_113]])), const<i32>(106), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_114]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v7]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_115]])), const<i32>(107), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_116]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_117]])), const<i32>(109), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_118]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_119]])), const<i32>(110), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_120]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v3]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_121]])), const<i32>(111), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_122]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_123]])), const<i32>(113), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_124]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_125]])), const<i32>(114), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_126]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_127]])), const<i32>(115), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_128]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v3]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_129]])), const<i32>(116), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_130]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v4]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_131]])), const<i32>(117), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_132]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v5]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_133]])), const<i32>(118), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_134]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v6]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_135]])), const<i32>(119), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_136]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i1]])))), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v7]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_137]])), const<i32>(120), array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_138]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_array_ref]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
