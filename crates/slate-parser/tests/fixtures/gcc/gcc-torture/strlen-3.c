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
// DEFAULT-NEXT:     global %1 a: array<array<array<i8, 9>, 3>, 2> [storage=static] [const] [align=16] = aggregate<array<array<array<i8, 9>, 3>, 2>, zero_fill=false>(index0 = aggregate<array<array<i8, 9>, 3>, zero_fill=true>(index0 = code_units<array<i8, 9>>([49, 0, 0, 0, 0, 0, 0, 0, 0]), index1 = code_units<array<i8, 9>>([49, 0, 50, 0, 0, 0, 0, 0, 0])), index1 = aggregate<array<array<i8, 9>, 3>, zero_fill=true>(index0 = code_units<array<i8, 9>>([49, 50, 0, 51, 0, 0, 0, 0, 0]), index1 = code_units<array<i8, 9>>([49, 50, 51, 0, 52, 0, 0, 0, 0]))) [linkage=internal];
// DEFAULT-NEXT:     global %2 v0: volatile i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %3 v1: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %4 v2: volatile i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %5 v3: volatile i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %6 v4: volatile i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %7 v5: volatile i32 [storage=static] = const<i32>(5) [linkage=external];
// DEFAULT-NEXT:     global %8 v6: volatile i32 [storage=static] = const<i32>(6) [linkage=external];
// DEFAULT-NEXT:     global %9 v7: volatile i32 [storage=static] = const<i32>(7) [linkage=external];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 48, 93, 91, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 49, 93, 91, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 50, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %46 .str46: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %49 .str49: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %51 .str51: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 50, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %57 .str57: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 49, 93, 91, 48, 93, 32, 43, 32, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %59 .str59: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %60 .str60: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %61 .str61: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %62 .str62: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %63 .str63: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %64 .str64: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %65 .str65: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %66 .str66: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %67 .str67: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %68 .str68: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %69 .str69: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %70 .str70: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %71 .str71: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %72 .str72: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %73 .str73: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 50, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %74 .str74: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %75 .str75: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 51, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %76 .str76: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %77 .str77: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 51, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %78 .str78: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %79 .str79: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %80 .str80: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %81 .str81: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %82 .str82: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %83 .str83: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %84 .str84: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %85 .str85: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 50, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %86 .str86: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %87 .str87: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 51, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %88 .str88: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %89 .str89: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 52, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %90 .str90: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %91 .str91: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 53, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %92 .str92: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %93 .str93: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 54, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %94 .str94: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %95 .str95: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 55, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %96 .str96: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %97 .str97: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %98 .str98: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %99 .str99: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %100 .str100: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %101 .str101: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %102 .str102: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %103 .str103: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %105 .str105: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %107 .str107: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 50, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 52, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 53, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %114 .str114: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 54, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 105, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %129 .str129: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 .str130: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %132 .str132: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %133 .str133: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %134 .str134: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %135 .str135: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %136 .str136: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %137 .str137: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %138 .str138: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %139 .str139: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %140 .str140: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %141 .str141: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %142 .str142: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %143 .str143: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %144 .str144: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %145 .str145: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %146 .str146: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %147 .str147: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %148 .str148: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %149 .str149: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %150 .str150: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %151 .str151: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %152 .str152: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %153 .str153: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %154 .str154: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %155 .str155: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 52, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %156 .str156: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %157 .str157: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 53, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %158 .str158: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %159 .str159: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 54, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %160 .str160: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %161 .str161: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 105, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 55, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @strlen(%20 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %22 @__builtin_printf(%21 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %25 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @test_array_ref() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %12 i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:         let %13 i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:         let %14 i3: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:         let %15 i4: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:         let %16 i5: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:         let %17 i6: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         let %18 i7: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%23)), const<i32>(34), array_decay<ptr<i8>, length=Some(21)>(%24));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(0)))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%26)), const<i32>(35), array_decay<ptr<i8>, length=Some(21)>(%27));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(1)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%28)), const<i32>(37), array_decay<ptr<i8>, length=Some(21)>(%29));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(1)))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%30)), const<i32>(38), array_decay<ptr<i8>, length=Some(21)>(%31));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(0)))), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%32)), const<i32>(40), array_decay<ptr<i8>, length=Some(25)>(%33));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(0)))), const<i32>(1)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%34)), const<i32>(41), array_decay<ptr<i8>, length=Some(25)>(%35));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(1)))), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%36)), const<i32>(43), array_decay<ptr<i8>, length=Some(25)>(%37));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(1)))), const<i32>(1)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%38)), const<i32>(44), array_decay<ptr<i8>, length=Some(25)>(%39));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(0)))), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%40)), const<i32>(46), array_decay<ptr<i8>, length=Some(29)>(%41));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(0)))), const<i32>(1)))), const<i32>(0)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%42)), const<i32>(47), array_decay<ptr<i8>, length=Some(29)>(%43));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(0)))), const<i32>(1)))), const<i32>(0)))), const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%44)), const<i32>(48), array_decay<ptr<i8>, length=Some(29)>(%45));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(0)))), const<i32>(1)))), const<i32>(0)))), const<i32>(3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%46)), const<i32>(49), array_decay<ptr<i8>, length=Some(29)>(%47));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(0)))), const<i32>(1)))), const<i32>(0)))), const<i32>(7))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%48)), const<i32>(50), array_decay<ptr<i8>, length=Some(29)>(%49));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(1)))), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%50)), const<i32>(52), array_decay<ptr<i8>, length=Some(29)>(%51));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(1)))), const<i32>(1)))), const<i32>(0)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%52)), const<i32>(53), array_decay<ptr<i8>, length=Some(29)>(%53));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(1)))), const<i32>(1)))), const<i32>(0)))), const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%54)), const<i32>(54), array_decay<ptr<i8>, length=Some(29)>(%55));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), const<i32>(1)))), const<i32>(1)))), const<i32>(0)))), const<i32>(7))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%56)), const<i32>(55), array_decay<ptr<i8>, length=Some(29)>(%57));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%58)), const<i32>(57), array_decay<ptr<i8>, length=Some(23)>(%59));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%60)), const<i32>(58), array_decay<ptr<i8>, length=Some(23)>(%61));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%62)), const<i32>(60), array_decay<ptr<i8>, length=Some(23)>(%63));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%64)), const<i32>(61), array_decay<ptr<i8>, length=Some(23)>(%65));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%11)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%66)), const<i32>(63), array_decay<ptr<i8>, length=Some(28)>(%67));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%68)), const<i32>(64), array_decay<ptr<i8>, length=Some(28)>(%69));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12)))), read<i32>(%12))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%70)), const<i32>(65), array_decay<ptr<i8>, length=Some(28)>(%71));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12)))), read<i32>(%13))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%72)), const<i32>(66), array_decay<ptr<i8>, length=Some(28)>(%73));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12)))), read<i32>(%14))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%74)), const<i32>(67), array_decay<ptr<i8>, length=Some(28)>(%75));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12)))), read<i32>(%14))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%76)), const<i32>(68), array_decay<ptr<i8>, length=Some(28)>(%77));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%78)), const<i32>(70), array_decay<ptr<i8>, length=Some(28)>(%79));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%80)), const<i32>(71), array_decay<ptr<i8>, length=Some(28)>(%81));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%12))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%82)), const<i32>(72), array_decay<ptr<i8>, length=Some(28)>(%83));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%13))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%84)), const<i32>(73), array_decay<ptr<i8>, length=Some(28)>(%85));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%14))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%86)), const<i32>(74), array_decay<ptr<i8>, length=Some(28)>(%87));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%88)), const<i32>(75), array_decay<ptr<i8>, length=Some(28)>(%89));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%16))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%90)), const<i32>(76), array_decay<ptr<i8>, length=Some(28)>(%91));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%17))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%92)), const<i32>(77), array_decay<ptr<i8>, length=Some(28)>(%93));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%18))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%94)), const<i32>(78), array_decay<ptr<i8>, length=Some(28)>(%95));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%11)))), read<i32>(%11)))), read<i32>(%12))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%96)), const<i32>(80), array_decay<ptr<i8>, length=Some(33)>(%97));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%12))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%98)), const<i32>(81), array_decay<ptr<i8>, length=Some(33)>(%99));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%18))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%100)), const<i32>(82), array_decay<ptr<i8>, length=Some(33)>(%101));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%11)))), read<i32>(%12))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%102)), const<i32>(84), array_decay<ptr<i8>, length=Some(33)>(%103));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%12))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%104)), const<i32>(85), array_decay<ptr<i8>, length=Some(33)>(%105));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%13))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%106)), const<i32>(86), array_decay<ptr<i8>, length=Some(33)>(%107));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%14))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%108)), const<i32>(87), array_decay<ptr<i8>, length=Some(33)>(%109));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%15))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%110)), const<i32>(88), array_decay<ptr<i8>, length=Some(33)>(%111));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%16))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%112)), const<i32>(89), array_decay<ptr<i8>, length=Some(33)>(%113));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%17))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%114)), const<i32>(90), array_decay<ptr<i8>, length=Some(33)>(%115));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%18))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%116)), const<i32>(91), array_decay<ptr<i8>, length=Some(33)>(%117));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%118)), const<i32>(93), array_decay<ptr<i8>, length=Some(23)>(%119));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%120)), const<i32>(94), array_decay<ptr<i8>, length=Some(23)>(%121));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%122)), const<i32>(96), array_decay<ptr<i8>, length=Some(23)>(%123));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%124)), const<i32>(97), array_decay<ptr<i8>, length=Some(23)>(%125));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%11)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%126)), const<i32>(99), array_decay<ptr<i8>, length=Some(28)>(%127));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%128)), const<i32>(100), array_decay<ptr<i8>, length=Some(28)>(%129));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%130)), const<i32>(102), array_decay<ptr<i8>, length=Some(28)>(%131));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%132)), const<i32>(103), array_decay<ptr<i8>, length=Some(28)>(%133));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%11)))), read<i32>(%11)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%134)), const<i32>(105), array_decay<ptr<i8>, length=Some(33)>(%135));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%11)))), read<i32>(%11)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%136)), const<i32>(106), array_decay<ptr<i8>, length=Some(33)>(%137));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%11)))), read<i32>(%11)))), read<i32, volatile>(%9))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%138)), const<i32>(107), array_decay<ptr<i8>, length=Some(33)>(%139));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12)))), read<i32>(%11)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%140)), const<i32>(109), array_decay<ptr<i8>, length=Some(33)>(%141));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12)))), read<i32>(%11)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%142)), const<i32>(110), array_decay<ptr<i8>, length=Some(33)>(%143));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%12)))), read<i32>(%11)))), read<i32, volatile>(%5))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%144)), const<i32>(111), array_decay<ptr<i8>, length=Some(33)>(%145));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%11)))), read<i32>(%11)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%146)), const<i32>(113), array_decay<ptr<i8>, length=Some(33)>(%147));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%148)), const<i32>(114), array_decay<ptr<i8>, length=Some(33)>(%149));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%150)), const<i32>(115), array_decay<ptr<i8>, length=Some(33)>(%151));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32, volatile>(%5))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%152)), const<i32>(116), array_decay<ptr<i8>, length=Some(33)>(%153));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32, volatile>(%6))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%154)), const<i32>(117), array_decay<ptr<i8>, length=Some(33)>(%155));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32, volatile>(%7))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%156)), const<i32>(118), array_decay<ptr<i8>, length=Some(33)>(%157));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32, volatile>(%8))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%158)), const<i32>(119), array_decay<ptr<i8>, length=Some(33)>(%159));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(9)>(deref(ptr_offset<ptr<const array<i8, 9>>, subtract=false, element=array<i8, 9>, overflow=ub>(array_decay<ptr<const array<i8, 9>>, length=Some(3)>(deref(ptr_offset<ptr<const array<array<i8, 9>, 3>>, subtract=false, element=array<array<i8, 9>, 3>, overflow=ub>(array_decay<ptr<const array<array<i8, 9>, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32>(%12)))), read<i32>(%11)))), read<i32, volatile>(%9))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%160)), const<i32>(120), array_decay<ptr<i8>, length=Some(33)>(%161));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
