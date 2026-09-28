/* Test for _Atomic in C11.  Basic execution tests for atomic loads
   and stores.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

extern void abort (void);
extern void exit (int);
extern int memcmp (const void *, const void *, __SIZE_TYPE__);

#define CMPLX(X, Y) __builtin_complex ((X), (Y))

#define TEST_SIMPLE_ASSIGN(TYPE, VALUE)				\
  do								\
    {								\
      static volatile _Atomic (TYPE) a, b = (TYPE) (VALUE);	\
      if (a != 0)						\
	abort ();						\
      if (b != ((TYPE) (VALUE)))				\
	abort ();						\
      if ((a = b) != ((TYPE) (VALUE)))				\
	abort ();						\
      if (a != ((TYPE) (VALUE)))				\
	abort ();						\
    }								\
  while (0)

#define TEST_SIMPLE_ASSIGN_ARITH(VALUE)				\
  do								\
    {								\
      TEST_SIMPLE_ASSIGN (_Bool, (VALUE));			\
      TEST_SIMPLE_ASSIGN (char, (VALUE));			\
      TEST_SIMPLE_ASSIGN (signed char, (VALUE));		\
      TEST_SIMPLE_ASSIGN (unsigned char, (VALUE));		\
      TEST_SIMPLE_ASSIGN (signed short, (VALUE));		\
      TEST_SIMPLE_ASSIGN (unsigned short, (VALUE));		\
      TEST_SIMPLE_ASSIGN (signed int, (VALUE));			\
      TEST_SIMPLE_ASSIGN (unsigned int, (VALUE));		\
      TEST_SIMPLE_ASSIGN (signed long, (VALUE));		\
      TEST_SIMPLE_ASSIGN (unsigned long, (VALUE));		\
      TEST_SIMPLE_ASSIGN (signed long long, (VALUE));		\
      TEST_SIMPLE_ASSIGN (unsigned long long, (VALUE));		\
      TEST_SIMPLE_ASSIGN (float, (VALUE));			\
      TEST_SIMPLE_ASSIGN (double, (VALUE));			\
      TEST_SIMPLE_ASSIGN (long double, (VALUE));		\
      TEST_SIMPLE_ASSIGN (_Complex float, (VALUE));		\
      TEST_SIMPLE_ASSIGN (_Complex double, (VALUE));		\
      TEST_SIMPLE_ASSIGN (_Complex long double, (VALUE));	\
    }								\
  while (0)

static void
test_simple_assign (void)
{
  TEST_SIMPLE_ASSIGN_ARITH (0);
  TEST_SIMPLE_ASSIGN_ARITH (1);
  TEST_SIMPLE_ASSIGN_ARITH (2);
  TEST_SIMPLE_ASSIGN_ARITH (-1);
  TEST_SIMPLE_ASSIGN_ARITH (1ULL << 63);
  TEST_SIMPLE_ASSIGN_ARITH (1.5);
  TEST_SIMPLE_ASSIGN_ARITH (CMPLX (2.5, 3.5));
  static int i;
  TEST_SIMPLE_ASSIGN (int *, 0);
  TEST_SIMPLE_ASSIGN (int *, &i);
  struct s { short a[1024]; };
  struct s init, copy;
  _Atomic struct s s1, s2;
  for (int j = 0; j < 1024; j++)
    init.a[j] = j;
  copy = (s1 = init);
  if (memcmp (&init, &copy, sizeof init) != 0)
    abort ();
  copy = (s2 = s1);
  if (memcmp (&init, &copy, sizeof init) != 0)
    abort ();
  copy = s1;
  if (memcmp (&init, &copy, sizeof init) != 0)
    abort ();
  copy = s2;
  if (memcmp (&init, &copy, sizeof init) != 0)
    abort ();
}

int
main (void)
{
  test_simple_assign ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 a: array<i16, 1024>;
// DEFAULT-NEXT:     } [size=2048, align=2, offsets=[0]];
// DEFAULT-NEXT:     global %4 a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %5 b: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %6 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %7 b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %8 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %9 b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %10 a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %11 b: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %12 a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %13 b: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %14 a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %15 b: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %16 a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %17 b: volatile atomic i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %18 a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %19 b: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %20 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %21 b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %22 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %23 b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %24 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %25 b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %26 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %27 b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %28 a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %29 b: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %30 a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %31 b: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %32 a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %33 b: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %34 a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %35 b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %36 a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %37 b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %38 a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %39 b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %40 a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %41 b: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %42 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %43 b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %44 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %45 b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %46 a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %47 b: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %48 a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %49 b: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %50 a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %51 b: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %52 a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %53 b: volatile atomic i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %54 a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %55 b: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %56 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %57 b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %58 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %59 b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %60 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %61 b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %62 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %63 b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %64 a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %65 b: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %66 a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %67 b: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %68 a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %69 b: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %70 a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %71 b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %72 a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %73 b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %74 a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %75 b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %76 a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %77 b: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %78 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %79 b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %80 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %81 b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %82 a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %83 b: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %84 a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %85 b: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %86 a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %87 b: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %88 a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %89 b: volatile atomic i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %90 a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %91 b: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %92 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %93 b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %94 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %95 b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %96 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %97 b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %98 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %99 b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %100 a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %101 b: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %102 a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %103 b: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %104 a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %105 b: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %106 a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %107 b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %108 a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %109 b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %110 a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %111 b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %112 a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %113 b: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %114 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %115 b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %116 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %117 b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %118 a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %119 b: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %120 a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %121 b: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %122 a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %123 b: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %124 a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %125 b: volatile atomic i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %126 a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %127 b: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %128 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %129 b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %130 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %131 b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %132 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %133 b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %134 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %135 b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %136 a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %137 b: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %138 a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %139 b: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %140 a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %141 b: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %142 a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %143 b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %144 a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %145 b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %146 a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %147 b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %148 a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %149 b: volatile atomic bool [storage=static] = ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)), const<u64>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %150 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %151 b: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %152 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %153 b: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %154 a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %155 b: volatile atomic u8 [storage=static] = truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %156 a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %157 b: volatile atomic i16 [storage=static] = reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %158 a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %159 b: volatile atomic u16 [storage=static] = truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %160 a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %161 b: volatile atomic i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %162 a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %163 b: volatile atomic u32 [storage=static] = truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %164 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %165 b: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %166 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %167 b: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)) [linkage=internal];
// DEFAULT-NEXT:     global %168 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %169 b: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %170 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %171 b: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)) [linkage=internal];
// DEFAULT-NEXT:     global %172 a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %173 b: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %174 a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %175 b: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %176 a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %177 b: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %178 a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %179 b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %180 a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %181 b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %182 a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %183 b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %184 a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %185 b: volatile atomic bool [storage=static] = ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %186 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %187 b: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %188 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %189 b: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %190 a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %191 b: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %192 a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %193 b: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %194 a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %195 b: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %196 a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %197 b: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %198 a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %199 b: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %200 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %201 b: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %202 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %203 b: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %204 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %205 b: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %206 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %207 b: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %208 a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %209 b: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %210 a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %211 b: volatile atomic f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %212 a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %213 b: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %214 a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %215 b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))) [linkage=internal];
// DEFAULT-NEXT:     global %216 a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %217 b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %218 a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %219 b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(float_widen<f80, reason=explicit>(const<f64>(1.5))) [linkage=internal];
// DEFAULT-NEXT:     global %220 a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %221 b: volatile atomic bool [storage=static] = ne<complex<f64>, reason=explicit, exceptions=ignore>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0))) [linkage=internal];
// DEFAULT-NEXT:     global %222 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %223 b: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %224 a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %225 b: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %226 a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %227 b: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %228 a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %229 b: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %230 a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %231 b: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %232 a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %233 b: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %234 a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %235 b: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %236 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %237 b: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %238 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %239 b: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %240 a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %241 b: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %242 a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %243 b: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %244 a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %245 b: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %246 a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %247 b: volatile atomic f64 [storage=static] = complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))) [linkage=internal];
// DEFAULT-NEXT:     global %248 a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %249 b: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %250 a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %251 b: volatile atomic complex<f32> [storage=static] = complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=ignore>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))) [linkage=internal];
// DEFAULT-NEXT:     global %252 a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %253 b: volatile atomic complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)) [linkage=internal];
// DEFAULT-NEXT:     global %254 a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %255 b: volatile atomic complex<f80> [storage=static] = complex_convert<complex<f80>, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))) [linkage=internal];
// DEFAULT-NEXT:     global %256 i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %257 a: volatile atomic ptr<i32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %258 b: volatile atomic ptr<i32> [storage=static] = null<ptr<i32>> [linkage=internal];
// DEFAULT-NEXT:     global %259 a: volatile atomic ptr<i32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %260 b: volatile atomic ptr<i32> [storage=static] = addr_of<ptr<i32>>(%256) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%268 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @memcmp(%269 <unnamed>: ptr<const void>, %270 <unnamed>: ptr<const void>, %271 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @test_simple_assign() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %272
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %273
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%4)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%5)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%4, read<bool, volatile, atomic=seq_cst>(%5));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%5)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%4)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %274
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%6)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%7)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%6, read<i8, volatile, atomic=seq_cst>(%7));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%7)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%6)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %275
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%8)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%8, read<i8, volatile, atomic=seq_cst>(%9));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%8)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %276
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%10))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%11))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%10, read<u8, volatile, atomic=seq_cst>(%11));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%11))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%10))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %277
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%12)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%13)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%12, read<i16, volatile, atomic=seq_cst>(%13));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%13)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%12)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %278
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%14))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%15))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%14, read<u16, volatile, atomic=seq_cst>(%15));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%15))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%14))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %279
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%16), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%17), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%16, read<i32, volatile, atomic=seq_cst>(%17));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%17), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%16), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %280
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%18), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%19), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%18, read<u32, volatile, atomic=seq_cst>(%19));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%19), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%18), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %281
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%20), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%21), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%20, read<i64, volatile, atomic=seq_cst>(%21));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%21), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%20), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %282
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%22), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%23), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%22, read<u64, volatile, atomic=seq_cst>(%23));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%23), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%22), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %283
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%24), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%25), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%24, read<i64, volatile, atomic=seq_cst>(%25));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%25), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%24), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %284
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%26), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%27), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%26, read<u64, volatile, atomic=seq_cst>(%27));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%27), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%26), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %285
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%28), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%29), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%28, read<f32, volatile, atomic=seq_cst>(%29));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%29), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%28), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %286
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%30), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%31), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%30, read<f64, volatile, atomic=seq_cst>(%31));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%31), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%30), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %287
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%32), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%33), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%32, read<f80, volatile, atomic=seq_cst>(%33));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%33), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%32), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %288
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%34), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%35), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%34, read<complex<f32>, volatile, atomic=seq_cst>(%35));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%35), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%34), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %289
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%36), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%37), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%36, read<complex<f64>, volatile, atomic=seq_cst>(%37));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%37), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%36), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %290
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%38), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%39), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%38, read<complex<f80>, volatile, atomic=seq_cst>(%39));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%39), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%38), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %291
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %292
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%40)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%41)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%40, read<bool, volatile, atomic=seq_cst>(%41));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%41)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%40)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %293
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%42)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%43)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%42, read<i8, volatile, atomic=seq_cst>(%43));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%43)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%42)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %294
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%44)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%45)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%44, read<i8, volatile, atomic=seq_cst>(%45));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%45)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%44)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %295
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%46))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%47))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%46, read<u8, volatile, atomic=seq_cst>(%47));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%47))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%46))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %296
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%48)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%49)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%48, read<i16, volatile, atomic=seq_cst>(%49));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%49)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%48)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %297
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%50))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%51))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%50, read<u16, volatile, atomic=seq_cst>(%51));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%51))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%50))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %298
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%52), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%53), const<i32>(1))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%52, read<i32, volatile, atomic=seq_cst>(%53));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%53), const<i32>(1))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%52), const<i32>(1))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %299
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%54), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%55), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%54, read<u32, volatile, atomic=seq_cst>(%55));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%55), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%54), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %300
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%56), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%57), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%56, read<i64, volatile, atomic=seq_cst>(%57));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%57), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%56), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %301
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%58), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%59), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%58, read<u64, volatile, atomic=seq_cst>(%59));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%59), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%58), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %302
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%60), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%61), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%60, read<i64, volatile, atomic=seq_cst>(%61));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%61), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%60), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %303
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%62), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%63), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%62, read<u64, volatile, atomic=seq_cst>(%63));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%63), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%62), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %304
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%64), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%65), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%64, read<f32, volatile, atomic=seq_cst>(%65));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%65), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%64), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %305
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%66), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%67), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%66, read<f64, volatile, atomic=seq_cst>(%67));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%67), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%66), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %306
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%68), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%69), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%68, read<f80, volatile, atomic=seq_cst>(%69));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%69), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%68), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %307
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%70), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%71), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%70, read<complex<f32>, volatile, atomic=seq_cst>(%71));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%71), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%70), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %308
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%72), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%73), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%72, read<complex<f64>, volatile, atomic=seq_cst>(%73));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%73), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%72), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %309
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%74), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%75), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%74, read<complex<f80>, volatile, atomic=seq_cst>(%75));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%75), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%74), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %310
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %311
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%76)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%77)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%76, read<bool, volatile, atomic=seq_cst>(%77));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%77)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%76)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %312
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%78)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%79)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%78, read<i8, volatile, atomic=seq_cst>(%79));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%79)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%78)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %313
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%80)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%81)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%80, read<i8, volatile, atomic=seq_cst>(%81));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%81)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%80)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %314
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%82))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%83))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%82, read<u8, volatile, atomic=seq_cst>(%83));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%83))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%82))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %315
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%84)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%85)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%84, read<i16, volatile, atomic=seq_cst>(%85));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%85)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%84)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %316
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%86))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%87))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%86, read<u16, volatile, atomic=seq_cst>(%87));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%87))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%86))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %317
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%88), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%89), const<i32>(2))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%88, read<i32, volatile, atomic=seq_cst>(%89));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%89), const<i32>(2))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%88), const<i32>(2))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %318
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%90), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%91), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%90, read<u32, volatile, atomic=seq_cst>(%91));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%91), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%90), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %319
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%92), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%93), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%92, read<i64, volatile, atomic=seq_cst>(%93));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%93), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%92), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %320
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%94), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%95), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%94, read<u64, volatile, atomic=seq_cst>(%95));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%95), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%94), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %321
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%96), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%97), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%96, read<i64, volatile, atomic=seq_cst>(%97));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%97), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%96), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %322
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%98), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%99), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%98, read<u64, volatile, atomic=seq_cst>(%99));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%99), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%98), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %323
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%100), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%101), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%100, read<f32, volatile, atomic=seq_cst>(%101));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%101), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%100), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %324
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%102), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%103), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%102, read<f64, volatile, atomic=seq_cst>(%103));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%103), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%102), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %325
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%104), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%105), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%104, read<f80, volatile, atomic=seq_cst>(%105));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%105), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%104), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %326
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%106), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%107), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%106, read<complex<f32>, volatile, atomic=seq_cst>(%107));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%107), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%106), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %327
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%108), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%109), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%108, read<complex<f64>, volatile, atomic=seq_cst>(%109));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%109), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%108), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %328
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%110), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%111), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%110, read<complex<f80>, volatile, atomic=seq_cst>(%111));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%111), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%110), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %329
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %330
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%112)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%113)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%112, read<bool, volatile, atomic=seq_cst>(%113));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%113)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%112)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %331
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%114)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%115)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%114, read<i8, volatile, atomic=seq_cst>(%115));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%115)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%114)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %332
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%116)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%117)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%116, read<i8, volatile, atomic=seq_cst>(%117));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%117)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%116)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %333
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%118))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%119))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%118, read<u8, volatile, atomic=seq_cst>(%119));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%119))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%118))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %334
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%120)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%121)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%120, read<i16, volatile, atomic=seq_cst>(%121));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%121)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%120)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %335
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%122))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%123))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%122, read<u16, volatile, atomic=seq_cst>(%123));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%123))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%122))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %336
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%124), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%125), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%124, read<i32, volatile, atomic=seq_cst>(%125));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%125), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%124), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %337
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%126), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%127), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%126, read<u32, volatile, atomic=seq_cst>(%127));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%127), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%126), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %338
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%128), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%129), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%128, read<i64, volatile, atomic=seq_cst>(%129));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%129), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%128), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %339
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%130), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%131), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%130, read<u64, volatile, atomic=seq_cst>(%131));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%131), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%130), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %340
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%132), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%133), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%132, read<i64, volatile, atomic=seq_cst>(%133));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%133), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%132), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %341
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%134), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%135), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%134, read<u64, volatile, atomic=seq_cst>(%135));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%135), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%134), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %342
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%136), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%137), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%136, read<f32, volatile, atomic=seq_cst>(%137));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%137), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%136), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %343
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%138), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%139), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%138, read<f64, volatile, atomic=seq_cst>(%139));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%139), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%138), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %344
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%140), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%141), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%140, read<f80, volatile, atomic=seq_cst>(%141));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%141), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%140), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %345
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%142), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%143), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%142, read<complex<f32>, volatile, atomic=seq_cst>(%143));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%143), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%142), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %346
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%144), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%145), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%144, read<complex<f64>, volatile, atomic=seq_cst>(%145));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%145), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%144), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %347
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%146), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%147), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%146, read<complex<f80>, volatile, atomic=seq_cst>(%147));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%147), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%146), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %348
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %349
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%148)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%149)), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)), const<u64>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%148, read<bool, volatile, atomic=seq_cst>(%149));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%149)), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)), const<u64>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%148)), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)), const<u64>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %350
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%150)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%151)), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%150, read<i8, volatile, atomic=seq_cst>(%151));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%151)), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%150)), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %351
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%152)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%153)), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%152, read<i8, volatile, atomic=seq_cst>(%153));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%153)), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%152)), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %352
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%154))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%155))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%154, read<u8, volatile, atomic=seq_cst>(%155));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%155))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%154))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %353
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%156)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%157)), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%156, read<i16, volatile, atomic=seq_cst>(%157));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%157)), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%156)), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %354
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%158))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%159))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%158, read<u16, volatile, atomic=seq_cst>(%159));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%159))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%158))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %355
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%160), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%161), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%160, read<i32, volatile, atomic=seq_cst>(%161));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%161), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%160), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %356
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%162), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%163), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%162, read<u32, volatile, atomic=seq_cst>(%163));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%163), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%162), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %357
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%164), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%165), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%164, read<i64, volatile, atomic=seq_cst>(%165));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%165), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%164), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %358
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%166), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%167), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%166, read<u64, volatile, atomic=seq_cst>(%167));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%167), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%166), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %359
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%168), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%169), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%168, read<i64, volatile, atomic=seq_cst>(%169));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%169), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%168), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %360
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%170), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%171), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%170, read<u64, volatile, atomic=seq_cst>(%171));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%171), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%170), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %361
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%172), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%173), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%172, read<f32, volatile, atomic=seq_cst>(%173));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%173), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%172), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %362
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%174), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%175), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%174, read<f64, volatile, atomic=seq_cst>(%175));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%175), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%174), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %363
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%176), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%177), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%176, read<f80, volatile, atomic=seq_cst>(%177));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%177), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%176), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %364
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%178), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%179), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%178, read<complex<f32>, volatile, atomic=seq_cst>(%179));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%179), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%178), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %365
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%180), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%181), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%180, read<complex<f64>, volatile, atomic=seq_cst>(%181));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%181), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%180), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %366
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%182), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%183), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%182, read<complex<f80>, volatile, atomic=seq_cst>(%183));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%183), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%182), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %367
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %368
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%184)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%185)), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%184, read<bool, volatile, atomic=seq_cst>(%185));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%185)), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%184)), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %369
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%186)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%187)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%186, read<i8, volatile, atomic=seq_cst>(%187));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%187)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%186)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %370
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%188)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%189)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%188, read<i8, volatile, atomic=seq_cst>(%189));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%189)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%188)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %371
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%190))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%191))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%190, read<u8, volatile, atomic=seq_cst>(%191));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%191))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%190))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %372
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%192)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%193)), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%192, read<i16, volatile, atomic=seq_cst>(%193));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%193)), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%192)), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %373
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%194))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%195))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%194, read<u16, volatile, atomic=seq_cst>(%195));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%195))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%194))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %374
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%196), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%197), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%196, read<i32, volatile, atomic=seq_cst>(%197));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%197), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%196), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %375
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%198), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%199), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%198, read<u32, volatile, atomic=seq_cst>(%199));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%199), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%198), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %376
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%200), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%201), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%200, read<i64, volatile, atomic=seq_cst>(%201));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%201), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%200), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %377
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%202), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%203), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%202, read<u64, volatile, atomic=seq_cst>(%203));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%203), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%202), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %378
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%204), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%205), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%204, read<i64, volatile, atomic=seq_cst>(%205));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%205), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%204), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %379
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%206), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%207), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%206, read<u64, volatile, atomic=seq_cst>(%207));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%207), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%206), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %380
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%208), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%209), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%208, read<f32, volatile, atomic=seq_cst>(%209));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%209), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%208), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %381
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%210), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%211), const<f64>(1.5))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%210, read<f64, volatile, atomic=seq_cst>(%211));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%211), const<f64>(1.5))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%210), const<f64>(1.5))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %382
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%212), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%213), float_widen<f80, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%212, read<f80, volatile, atomic=seq_cst>(%213));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%213), float_widen<f80, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%212), float_widen<f80, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %383
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%214), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%215), real_to_complex<complex<f32>, reason=explicit>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%214, read<complex<f32>, volatile, atomic=seq_cst>(%215));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%215), real_to_complex<complex<f32>, reason=explicit>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%214), real_to_complex<complex<f32>, reason=explicit>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %384
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%216), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%217), real_to_complex<complex<f64>, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%216, read<complex<f64>, volatile, atomic=seq_cst>(%217));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%217), real_to_complex<complex<f64>, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%216), real_to_complex<complex<f64>, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %385
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%218), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%219), real_to_complex<complex<f80>, reason=explicit>(float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%218, read<complex<f80>, volatile, atomic=seq_cst>(%219));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%219), real_to_complex<complex<f80>, reason=explicit>(float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%218), real_to_complex<complex<f80>, reason=explicit>(float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %386
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %387
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%220)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%221)), from_bool<i32, reason=promotion>(ne<complex<f64>, reason=explicit, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%220, read<bool, volatile, atomic=seq_cst>(%221));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%221)), from_bool<i32, reason=promotion>(ne<complex<f64>, reason=explicit, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%220)), from_bool<i32, reason=promotion>(ne<complex<f64>, reason=explicit, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %388
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%222)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%223)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%222, read<i8, volatile, atomic=seq_cst>(%223));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%223)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%222)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %389
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%224)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%225)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%224, read<i8, volatile, atomic=seq_cst>(%225));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%225)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%224)), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %390
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%226))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%227))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%226, read<u8, volatile, atomic=seq_cst>(%227));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%227))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%226))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %391
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%228)), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%229)), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%228, read<i16, volatile, atomic=seq_cst>(%229));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%229)), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%228)), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %392
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%230))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%231))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%230, read<u16, volatile, atomic=seq_cst>(%231));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%231))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%230))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %393
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%232), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%233), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%232, read<i32, volatile, atomic=seq_cst>(%233));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%233), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%232), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %394
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%234), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%235), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%234, read<u32, volatile, atomic=seq_cst>(%235));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%235), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%234), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %395
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%236), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%237), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%236, read<i64, volatile, atomic=seq_cst>(%237));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%237), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%236), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %396
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%238), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%239), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%238, read<u64, volatile, atomic=seq_cst>(%239));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%239), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%238), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %397
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%240), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%241), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%240, read<i64, volatile, atomic=seq_cst>(%241));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%241), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%240), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %398
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%242), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%243), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%242, read<u64, volatile, atomic=seq_cst>(%243));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%243), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%242), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %399
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%244), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%245), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%244, read<f32, volatile, atomic=seq_cst>(%245));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%245), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%244), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %400
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%246), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%247), complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%246, read<f64, volatile, atomic=seq_cst>(%247));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%247), complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%246), complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %401
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%248), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%249), float_widen<f80, reason=explicit>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%248, read<f80, volatile, atomic=seq_cst>(%249));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%249), float_widen<f80, reason=explicit>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%248), float_widen<f80, reason=explicit>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %402
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%250), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%251), complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%250, read<complex<f32>, volatile, atomic=seq_cst>(%251));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%251), complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%250), complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %403
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%252), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%253), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%252, read<complex<f64>, volatile, atomic=seq_cst>(%253));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%253), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%252), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %404
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%254), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%255), complex_convert<complex<f80>, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%254, read<complex<f80>, volatile, atomic=seq_cst>(%255));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%255), complex_convert<complex<f80>, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%254), complex_convert<complex<f80>, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %405
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%257), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%258), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 write<ptr<i32>, volatile, atomic=seq_cst>(%257, read<ptr<i32>, volatile, atomic=seq_cst>(%258));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%258), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%257), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %406
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%259), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%260), addr_of<ptr<i32>>(%256))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 write<ptr<i32>, volatile, atomic=seq_cst>(%259, read<ptr<i32>, volatile, atomic=seq_cst>(%260));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%260), addr_of<ptr<i32>>(%256))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%259), addr_of<ptr<i32>>(%256))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         let %262 init: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %263 copy: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %264 s1: atomic @type0 [storage=automatic];
// DEFAULT-NEXT:         let %265 s2: atomic @type0 [storage=automatic];
// DEFAULT-NEXT:         for %407
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %266 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%266), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %408: i32 [synthetic] = read<i32>(%266);
// DEFAULT-NEXT:                 let %409: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%408), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%266, read<i32>(%409));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(field0(%262)), read<i32>(%266))), truncate<i16, reason=assign, fits=unknown>(read<i32>(%266)));
// DEFAULT-NEXT:         write<@type0, atomic=seq_cst>(%264, copy<@type0, reason=assign>(read<@type0>(%262)));
// DEFAULT-NEXT:         write<@type0>(%263, copy<@type0, reason=assign>(copy<@type0, reason=assign>(read<@type0>(%262))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(%262)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(%263)), const<u64>(2048)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<@type0, atomic=seq_cst>(%265, copy<@type0, reason=assign>(read<@type0, atomic=seq_cst>(%264)));
// DEFAULT-NEXT:         write<@type0>(%263, copy<@type0, reason=assign>(copy<@type0, reason=assign>(read<@type0, atomic=seq_cst>(%264))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(%262)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(%263)), const<u64>(2048)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<@type0>(%263, copy<@type0, reason=assign>(read<@type0, atomic=seq_cst>(%264)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(%262)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(%263)), const<u64>(2048)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<@type0>(%263, copy<@type0, reason=assign>(read<@type0, atomic=seq_cst>(%265)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(%262)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(%263)), const<u64>(2048)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %267 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
