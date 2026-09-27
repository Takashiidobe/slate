/* Test for _Atomic in C11.  Basic execution tests for atomic
   increment and decrement.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

extern void abort (void);
extern void exit (int);

#define TEST_INCDEC(TYPE, VALUE, PREOP, POSTOP, PRE_P, CHANGE)		\
  do									\
    {									\
      static volatile _Atomic (TYPE) a = (TYPE) (VALUE);		\
      if (PREOP a POSTOP != (PRE_P					\
			     ? (TYPE) ((TYPE) (VALUE) + (CHANGE))	\
			     : (TYPE) (VALUE)))				\
	abort ();							\
      if (a != (TYPE) ((TYPE) (VALUE) + (CHANGE)))			\
	abort ();							\
    }									\
  while (0)

#define TEST_INCDEC_ARITH(VALUE, PREOP, POSTOP, PRE_P, CHANGE)		\
  do									\
    {									\
      TEST_INCDEC (_Bool, (VALUE), PREOP, POSTOP, (PRE_P), (CHANGE));	\
      TEST_INCDEC (char, (VALUE), PREOP, POSTOP, (PRE_P), (CHANGE));	\
      TEST_INCDEC (signed char, (VALUE), PREOP, POSTOP, (PRE_P),	\
		   (CHANGE));						\
      TEST_INCDEC (unsigned char, (VALUE), PREOP, POSTOP, (PRE_P),	\
		   (CHANGE));						\
      TEST_INCDEC (signed short, (VALUE), PREOP, POSTOP, (PRE_P),	\
		   (CHANGE));						\
      TEST_INCDEC (unsigned short, (VALUE), PREOP, POSTOP, (PRE_P),	\
		   (CHANGE));						\
      TEST_INCDEC (signed int, (VALUE), PREOP, POSTOP, (PRE_P),		\
		   (CHANGE));						\
      TEST_INCDEC (unsigned int, (VALUE), PREOP, POSTOP, (PRE_P),	\
		   (CHANGE));						\
      TEST_INCDEC (signed long, (VALUE), PREOP, POSTOP, (PRE_P),	\
		   (CHANGE));						\
      TEST_INCDEC (unsigned long, (VALUE), PREOP, POSTOP, (PRE_P),	\
		   (CHANGE));						\
      TEST_INCDEC (signed long long, (VALUE), PREOP, POSTOP, (PRE_P),	\
		   (CHANGE));						\
      TEST_INCDEC (unsigned long long, (VALUE), PREOP, POSTOP, (PRE_P), \
		   (CHANGE));						\
      TEST_INCDEC (float, (VALUE), PREOP, POSTOP, (PRE_P), (CHANGE));	\
      TEST_INCDEC (double, (VALUE), PREOP, POSTOP, (PRE_P), (CHANGE));	\
      TEST_INCDEC (long double, (VALUE), PREOP, POSTOP, (PRE_P),	\
		   (CHANGE));						\
    }									\
  while (0)

#define TEST_ALL_INCDEC_ARITH(VALUE)		\
  do						\
    {						\
      TEST_INCDEC_ARITH ((VALUE), ++, , 1, 1);	\
      TEST_INCDEC_ARITH ((VALUE), --, , 1, -1);	\
      TEST_INCDEC_ARITH ((VALUE), , ++, 0, 1);	\
      TEST_INCDEC_ARITH ((VALUE), , --, 0, -1);	\
    }						\
  while (0)

static void
test_incdec (void)
{
  TEST_ALL_INCDEC_ARITH (0);
  TEST_ALL_INCDEC_ARITH (1);
  TEST_ALL_INCDEC_ARITH (2);
  TEST_ALL_INCDEC_ARITH (-1);
  TEST_ALL_INCDEC_ARITH (1ULL << 60);
  TEST_ALL_INCDEC_ARITH (1.5);
  static int ia[2];
  TEST_INCDEC (int *, &ia[1], ++, , 1, 1);
  TEST_INCDEC (int *, &ia[1], --, , 1, -1);
  TEST_INCDEC (int *, &ia[1], , ++, 0, 1);
  TEST_INCDEC (int *, &ia[1], , --, 0, -1);
}

int
main (void)
{
  test_incdec ();
  exit (0);
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %3 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %4 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %5 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %6 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %7 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %8 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %9 a: volatile atomic i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %10 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %11 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %12 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %13 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %14 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %15 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %16 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %17 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %18 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %19 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %20 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %21 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %22 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %23 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %24 a: volatile atomic i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %25 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %26 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %27 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %28 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %29 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %30 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %31 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %32 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %33 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %34 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %35 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %36 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %37 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %38 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %39 a: volatile atomic i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %40 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %41 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %42 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %43 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %44 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %45 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %46 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %47 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %48 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %49 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %50 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %51 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %52 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %53 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %54 a: volatile atomic i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %55 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %56 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %57 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %58 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %59 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %60 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %61 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %62 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %63 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %64 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %65 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %66 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %67 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %68 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %69 a: volatile atomic i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %70 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %71 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %72 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %73 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %74 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %75 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %76 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %77 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %78 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %79 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %80 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %81 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %82 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %83 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %84 a: volatile atomic i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %85 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %86 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %87 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %88 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %89 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %90 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %91 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %92 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %93 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %94 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %95 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %96 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %97 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %98 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %99 a: volatile atomic i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %100 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %101 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %102 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %103 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %104 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %105 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %106 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %107 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %108 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %109 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %110 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %111 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %112 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %113 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %114 a: volatile atomic i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %115 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %116 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %117 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %118 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %119 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %120 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %121 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %122 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %123 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %124 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %125 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %126 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %127 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %128 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %129 a: volatile atomic i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %130 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %131 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %132 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %133 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %134 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %135 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %136 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %137 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %138 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %139 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %140 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %141 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %142 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %143 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %144 a: volatile atomic i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %145 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %146 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %147 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %148 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %149 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %150 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %151 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %152 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %153 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %154 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %155 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %156 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %157 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %158 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %159 a: volatile atomic i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %160 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %161 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %162 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %163 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %164 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %165 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %166 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %167 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %168 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %169 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %170 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %171 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %172 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %173 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %174 a: volatile atomic i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %175 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %176 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %177 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %178 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %179 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %180 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %181 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %182 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %183 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %184 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %185 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %186 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %187 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %188 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %189 a: volatile atomic i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %190 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %191 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %192 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %193 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %194 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %195 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %196 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %197 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %198 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %199 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %200 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %201 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %202 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %203 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %204 a: volatile atomic i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %205 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %206 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %207 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %208 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %209 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %210 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %211 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %212 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %213 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %214 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %215 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %216 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %217 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %218 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %219 a: volatile atomic i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %220 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %221 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %222 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %223 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %224 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %225 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %226 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %227 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %228 a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %229 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %230 a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %231 a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %232 a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %233 a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %234 a: volatile atomic i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %235 a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %236 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %237 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %238 a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %239 a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %240 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %241 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %242 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %243 a: volatile atomic bool [storage=static] = ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %244 a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %245 a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %246 a: volatile atomic u8 [storage=static] = truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %247 a: volatile atomic i16 [storage=static] = reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %248 a: volatile atomic u16 [storage=static] = truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %249 a: volatile atomic i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %250 a: volatile atomic u32 [storage=static] = truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %251 a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %252 a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %253 a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %254 a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %255 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %256 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %257 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %258 a: volatile atomic bool [storage=static] = ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %259 a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %260 a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %261 a: volatile atomic u8 [storage=static] = truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %262 a: volatile atomic i16 [storage=static] = reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %263 a: volatile atomic u16 [storage=static] = truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %264 a: volatile atomic i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %265 a: volatile atomic u32 [storage=static] = truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %266 a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %267 a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %268 a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %269 a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %270 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %271 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %272 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %273 a: volatile atomic bool [storage=static] = ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %274 a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %275 a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %276 a: volatile atomic u8 [storage=static] = truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %277 a: volatile atomic i16 [storage=static] = reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %278 a: volatile atomic u16 [storage=static] = truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %279 a: volatile atomic i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %280 a: volatile atomic u32 [storage=static] = truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %281 a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %282 a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %283 a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %284 a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %285 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %286 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %287 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %288 a: volatile atomic bool [storage=static] = ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %289 a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %290 a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %291 a: volatile atomic u8 [storage=static] = truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %292 a: volatile atomic i16 [storage=static] = reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %293 a: volatile atomic u16 [storage=static] = truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %294 a: volatile atomic i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %295 a: volatile atomic u32 [storage=static] = truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %296 a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %297 a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %298 a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %299 a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %300 a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %301 a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %302 a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %303 a: volatile atomic bool [storage=static] = ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %304 a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %305 a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %306 a: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %307 a: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %308 a: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %309 a: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %310 a: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %311 a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %312 a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %313 a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %314 a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %315 a: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %316 a: volatile atomic f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %317 a: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %318 a: volatile atomic bool [storage=static] = ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %319 a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %320 a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %321 a: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %322 a: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %323 a: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %324 a: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %325 a: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %326 a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %327 a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %328 a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %329 a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %330 a: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %331 a: volatile atomic f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %332 a: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %333 a: volatile atomic bool [storage=static] = ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %334 a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %335 a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %336 a: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %337 a: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %338 a: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %339 a: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %340 a: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %341 a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %342 a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %343 a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %344 a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %345 a: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %346 a: volatile atomic f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %347 a: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %348 a: volatile atomic bool [storage=static] = ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %349 a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %350 a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %351 a: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %352 a: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %353 a: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %354 a: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %355 a: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %356 a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %357 a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %358 a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %359 a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %360 a: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %361 a: volatile atomic f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %362 a: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %363 ia: array<i32, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %364 a: volatile atomic ptr<i32> [storage=static] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %365 a: volatile atomic ptr<i32> [storage=static] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %366 a: volatile atomic ptr<i32> [storage=static] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %367 a: volatile atomic ptr<i32> [storage=static] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%369 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @test_incdec() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %370
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %371
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %372
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %764: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%3, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%764)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%3)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %373
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %765: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%4, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%765)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%4)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %374
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %766: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%5, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%766)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%5)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %375
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %767: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%6, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%767))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%6))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %376
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %768: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%7, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%768)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%7)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %377
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %769: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%8, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%769))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%8))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %378
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %770: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%9, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%770), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(0), const<i32>(1)), const<i32>(0)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%9), add<i32, overflow=ub>(const<i32>(0), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %379
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %771: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%10, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%771), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%10), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %380
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %772: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%11, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%772), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%11), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %381
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %773: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%12, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%773), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%12), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %382
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %774: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%13, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%774), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%13), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %383
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %775: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%14, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%775), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%14), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %384
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %776: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%15, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%776), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%15), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %385
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %777: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%16, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%777), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%16), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %386
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %778: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%17, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%778), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%17), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %387
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %388
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %779: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%18, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%779)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%18)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %389
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %780: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%19, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%780)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%19)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %390
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %781: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%20, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%781)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%20)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %391
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %782: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%21, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%782))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%21))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %392
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %783: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%22, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%783)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%22)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %393
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %784: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%23, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%784))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %394
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %785: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%24, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%785), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%24), add<i32, overflow=ub>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %395
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %786: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%25, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%786), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%25), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %396
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %787: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%26, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%787), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%26), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %397
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %788: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%27, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%788), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%27), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %398
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %789: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%28, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%789), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%28), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %399
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %790: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%29, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%790), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%29), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %400
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %791: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%30, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%791), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%30), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %401
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %792: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%31, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%792), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%31), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %402
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %793: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%32, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%793), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%32), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %403
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %404
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %794: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%33, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%794)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%33)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %405
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %795: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%34, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%795)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%34)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %406
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %796: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%35, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%796)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%35)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %407
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %797: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%36, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%797))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%36))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %408
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %798: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%37, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%798)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%37)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %409
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %799: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%38, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%799))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%38))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %410
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %800: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%39, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%800), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(0), const<i32>(1)), const<i32>(0)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%39), add<i32, overflow=ub>(const<i32>(0), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %411
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %801: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%40, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%801), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%40), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %412
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %802: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%41, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%802), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%41), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %413
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %803: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%42, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%803), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%42), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %414
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %804: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%43, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%804), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%43), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %415
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %805: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%44, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%805), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%44), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %416
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %806: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%45, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%806), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%45), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %417
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %807: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%46, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%807), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%46), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %418
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %808: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%47, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%808), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%47), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %419
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %420
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %809: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%48, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%809)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%48)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %421
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %810: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%49, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%810)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%49)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %422
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %811: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%50, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%811)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%50)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %423
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %812: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%51, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%812))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%51))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %424
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %813: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%52, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%813)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%52)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %425
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %814: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%53, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%814))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%53))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %426
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %815: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%54, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%815), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%54), add<i32, overflow=ub>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %427
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %816: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%55, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%816), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%55), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %428
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %817: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%56, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%817), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%56), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %429
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %818: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%57, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%818), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%57), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %430
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %819: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%58, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%819), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%58), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %431
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %820: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%59, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%820), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%59), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %432
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %821: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%60, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%821), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%60), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %433
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %822: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%61, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%822), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%61), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %434
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %823: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%62, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%823), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%62), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %435
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %436
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %437
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %824: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%63, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%824)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%63)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %438
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %825: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%64, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%825)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%64)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %439
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %826: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%65, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%826)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%65)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %440
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %827: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%66, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%827))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%66))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %441
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %828: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%67, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%828)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%67)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %442
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %829: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%68, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%829))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%68))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %443
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %830: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%69, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%830), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%69), add<i32, overflow=ub>(const<i32>(1), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %444
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %831: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%70, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%831), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%70), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %445
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %832: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%71, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%832), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%71), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %446
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %833: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%72, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%833), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%72), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %447
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %834: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%73, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%834), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%73), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %448
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %835: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%74, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%835), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%74), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %449
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %836: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%75, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%836), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%75), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %450
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %837: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%76, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%837), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%76), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %451
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %838: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%77, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%838), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%77), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %452
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %453
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %839: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%78, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%839)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%78)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %454
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %840: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%79, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%840)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%79)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %455
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %841: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%80, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%841)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%80)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %456
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %842: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%81, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%842))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%81))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %457
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %843: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%82, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%843)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%82)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %458
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %844: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%83, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%844))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%83))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %459
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %845: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%84, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%845), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%84), add<i32, overflow=ub>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %460
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %846: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%85, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%846), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%85), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %461
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %847: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%86, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%847), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%86), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %462
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %848: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%87, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%848), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%87), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %463
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %849: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%88, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%849), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%88), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %464
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %850: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%89, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%850), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%89), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %465
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %851: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%90, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%851), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%90), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %466
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %852: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%91, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%852), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%91), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %467
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %853: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%92, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%853), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%92), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %468
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %469
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %854: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%93, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%854)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%93)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %470
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %855: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%94, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%855)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%94)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %471
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %856: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%95, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%856)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%95)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %472
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %857: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%96, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%857))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%96))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %473
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %858: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%97, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%858)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%97)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %474
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %859: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%98, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%859))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%98))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %475
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %860: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%99, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%860), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%99), add<i32, overflow=ub>(const<i32>(1), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %476
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %861: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%100, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%861), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%100), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %477
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %862: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%101, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%862), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%101), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %478
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %863: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%102, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%863), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%102), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %479
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %864: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%103, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%864), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%103), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %480
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %865: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%104, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%865), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%104), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %481
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %866: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%105, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%866), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%105), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %482
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %867: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%106, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%867), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%106), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %483
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %868: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%107, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%868), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%107), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %484
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %485
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %869: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%108, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%869)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%108)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %486
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %870: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%109, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%870)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%109)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %487
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %871: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%110, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%871)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%110)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %488
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %872: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%111, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%872))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%111))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %489
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %873: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%112, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%873)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%112)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %490
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %874: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%113, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%874))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%113))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %491
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %875: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%114, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%875), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%114), add<i32, overflow=ub>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %492
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %876: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%115, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%876), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%115), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %493
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %877: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%116, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%877), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%116), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %494
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %878: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%117, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%878), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%117), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %495
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %879: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%118, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%879), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%118), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %496
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %880: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%119, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%880), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%119), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %497
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %881: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%120, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%881), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%120), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %498
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %882: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%121, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%882), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%121), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %499
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %883: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%122, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%883), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%122), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %500
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %501
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %502
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %884: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%123, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%884)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%123)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %503
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %885: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%124, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%885)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%124)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %504
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %886: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%125, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%886)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%125)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %505
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %887: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%126, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%887))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%126))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %506
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %888: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%127, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%888)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%127)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %507
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %889: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%128, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%889))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%128))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %508
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %890: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%129, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%890), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)), const<i32>(2)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%129), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %509
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %891: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%130, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%891), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%130), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %510
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %892: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%131, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%892), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%131), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %511
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %893: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%132, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%893), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%132), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %512
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %894: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%133, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%894), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%133), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %513
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %895: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%134, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%895), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%134), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %514
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %896: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%135, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%896), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%135), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %515
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %897: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%136, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%897), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%136), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %516
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %898: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%137, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%898), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%137), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %517
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %518
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %899: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%138, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%899)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%138)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %519
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %900: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%139, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%900)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%139)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %520
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %901: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%140, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%901)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%140)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %521
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %902: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%141, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%902))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%141))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %522
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %903: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%142, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%903)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%142)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %523
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %904: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%143, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%904))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%143))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %524
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %905: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%144, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%905), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(2), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(2)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%144), add<i32, overflow=ub>(const<i32>(2), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %525
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %906: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%145, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%906), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%145), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %526
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %907: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%146, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%907), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%146), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %527
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %908: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%147, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%908), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%147), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %528
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %909: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%148, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%909), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%148), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %529
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %910: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%149, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%910), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%149), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %530
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %911: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%150, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%911), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%150), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %531
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %912: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%151, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%912), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%151), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %532
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %913: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%152, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%913), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%152), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %533
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %534
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %914: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%153, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%914)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%153)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %535
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %915: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%154, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%915)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%154)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %536
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %916: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%155, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%916)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%155)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %537
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %917: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%156, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%917))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%156))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %538
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %918: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%157, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%918)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%157)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %539
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %919: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%158, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%919))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%158))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %540
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %920: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%159, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%920), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)), const<i32>(2)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%159), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %541
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %921: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%160, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%921), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%160), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %542
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %922: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%161, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%922), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%161), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %543
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %923: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%162, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%923), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%162), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %544
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %924: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%163, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%924), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%163), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %545
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %925: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%164, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%925), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%164), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %546
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %926: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%165, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%926), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%165), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %547
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %927: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%166, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%927), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%166), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %548
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %928: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%167, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%928), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%167), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %549
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %550
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %929: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%168, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%929)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%168)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %551
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %930: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%169, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%930)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%169)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %552
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %931: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%170, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%931)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%170)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %553
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %932: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%171, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%932))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%171))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %554
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %933: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%172, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%933)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%172)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %555
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %934: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%173, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%934))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%173))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %556
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %935: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%174, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%935), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(2), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(2)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%174), add<i32, overflow=ub>(const<i32>(2), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %557
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %936: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%175, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%936), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%175), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %558
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %937: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%176, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%937), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%176), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %559
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %938: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%177, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%938), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%177), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %560
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %939: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%178, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%939), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%178), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %561
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %940: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%179, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%940), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%179), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %562
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %941: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%180, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%941), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%180), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %563
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %942: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%181, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%942), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%181), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %564
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %943: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%182, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%943), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%182), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %565
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %566
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %567
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %944: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%183, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%944)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%183)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %568
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %945: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%184, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%945)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%184)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %569
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %946: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%185, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%946)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%185)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %570
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %947: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%186, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%947))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%186))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %571
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %948: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%187, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%948)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%187)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %572
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %949: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%188, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%949))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%188))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %573
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %950: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%189, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%950), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%189), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %574
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %951: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%190, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%951), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%190), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %575
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %952: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%191, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%952), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%191), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %576
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %953: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%192, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%953), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%192), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %577
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %954: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%193, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%954), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%193), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %578
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %955: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%194, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%955), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%194), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %579
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %956: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%195, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%956), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%195), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %580
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %957: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%196, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%957), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%196), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %581
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %958: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%197, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%958), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%197), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %582
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %583
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %959: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%198, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%959)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%198)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %584
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %960: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%199, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%960)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%199)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %585
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %961: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%200, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%961)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%200)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %586
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %962: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%201, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%962))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%201))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %587
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %963: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%202, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%963)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%202)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %588
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %964: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%203, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%964))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%203))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %589
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %965: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%204, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%965), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%204), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %590
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %966: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%205, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%966), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%205), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %591
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %967: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%206, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%967), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%206), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %592
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %968: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%207, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%968), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%207), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %593
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %969: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%208, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%969), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%208), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %594
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %970: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%209, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%970), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%209), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %595
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %971: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%210, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%971), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%210), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %596
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %972: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%211, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%972), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%211), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %597
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %973: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%212, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%973), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%212), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %598
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %599
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %974: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%213, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%974)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%213)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %600
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %975: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%214, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%975)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%214)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %601
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %976: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%215, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%976)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%215)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %602
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %977: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%216, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%977))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%216))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %603
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %978: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%217, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%978)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%217)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %604
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %979: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%218, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%979))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%218))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %605
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %980: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%219, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%980), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%219), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %606
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %981: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%220, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%981), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%220), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %607
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %982: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%221, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%982), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%221), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %608
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %983: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%222, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%983), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%222), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %609
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %984: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%223, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%984), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%223), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %610
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %985: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%224, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%985), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%224), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %611
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %986: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%225, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%986), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%225), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %612
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %987: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%226, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%987), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%226), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %613
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %988: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%227, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%988), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%227), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %614
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %615
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %989: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%228, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%989)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%228)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %616
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %990: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%229, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%990)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%229)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %617
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %991: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%230, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%991)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%230)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %618
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %992: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%231, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%992))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%231))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %619
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %993: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%232, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%993)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%232)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %620
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %994: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%233, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%994))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %621
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %995: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%234, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%995), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%234), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %622
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %996: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%235, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%996), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%235), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %623
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %997: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%236, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%997), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%236), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %624
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %998: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%237, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%998), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%237), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %625
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %999: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%238, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%999), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%238), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %626
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1000: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%239, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1000), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%239), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %627
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1001: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%240, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%1001), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%240), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %628
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1002: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%241, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%1002), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%241), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %629
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1003: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%242, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%1003), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%242), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %630
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %631
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %632
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1004: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%243, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%1004)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%243)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %633
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1005: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%244, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1005)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%244)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %634
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1006: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%245, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1006)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%245)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %635
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1007: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%246, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1007))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%246))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %636
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1008: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%247, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1008)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%247)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %637
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1009: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%248, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1009))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%248))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %638
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1010: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%249, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%1010), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), const<i32>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%249), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %639
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1011: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%250, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%1011), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%250), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %640
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1012: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%251, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1012), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%251), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %641
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1013: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%252, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1013), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%252), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %642
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1014: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%253, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1014), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%253), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %643
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1015: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%254, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1015), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%254), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %644
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1016: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%255, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%1016), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%255), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %645
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1017: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%256, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%1017), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%256), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %646
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1018: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%257, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%1018), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%257), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %647
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %648
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1019: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%258, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%1019)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%258)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %649
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1020: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%259, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1020)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%259)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %650
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1021: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%260, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1021)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%260)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %651
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1022: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%261, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1022))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%261))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %652
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1023: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%262, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1023)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%262)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %653
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1024: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%263, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1024))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%263))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %654
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1025: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%264, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%1025), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), neg<i32, overflow=ub>(const<i32>(1))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%264), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %655
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1026: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%265, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%1026), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%265), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %656
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1027: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%266, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1027), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%266), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %657
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1028: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%267, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1028), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%267), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %658
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1029: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%268, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1029), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%268), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %659
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1030: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%269, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1030), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%269), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %660
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1031: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%270, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%1031), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%270), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %661
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1032: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%271, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%1032), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%271), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %662
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1033: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%272, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%1033), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%272), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %663
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %664
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1034: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%273, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%1034)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%273)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %665
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1035: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%274, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1035)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%274)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %666
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1036: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%275, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1036)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%275)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %667
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1037: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%276, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1037))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%276))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %668
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1038: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%277, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1038)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%277)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %669
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1039: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%278, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1039))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%278))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %670
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1040: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%279, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%1040), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), const<i32>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%279), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %671
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1041: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%280, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%1041), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%280), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %672
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1042: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%281, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1042), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%281), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %673
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1043: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%282, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1043), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%282), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %674
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1044: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%283, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1044), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%283), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %675
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1045: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%284, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1045), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%284), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %676
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1046: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%285, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%1046), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%285), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %677
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1047: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%286, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%1047), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%286), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %678
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1048: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%287, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%1048), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%287), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %679
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %680
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1049: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%288, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%1049)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%288)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %681
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1050: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%289, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1050)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%289)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %682
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1051: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%290, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1051)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%290)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %683
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1052: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%291, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1052))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%291))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %684
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1053: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%292, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1053)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%292)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %685
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1054: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%293, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1054))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%293))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %686
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1055: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%294, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%1055), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), neg<i32, overflow=ub>(const<i32>(1))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%294), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %687
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1056: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%295, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%1056), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%295), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %688
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1057: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%296, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1057), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%296), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %689
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1058: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%297, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1058), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%297), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %690
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1059: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%298, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1059), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%298), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %691
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1060: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%299, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1060), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%299), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %692
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1061: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%300, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%1061), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%300), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %693
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1062: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%301, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%1062), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%301), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %694
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1063: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%302, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%1063), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%302), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %695
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %696
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %697
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1064: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%303, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%1064)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%303)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %698
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1065: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%304, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1065)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%304)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %699
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1066: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%305, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1066)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%305)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %700
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1067: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%306, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1067))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%306))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %701
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1068: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%307, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1068)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%307)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %702
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1069: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%308, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1069))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%308))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %703
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1070: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%309, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%1070), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), const<i32>(1)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%309), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %704
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1071: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%310, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%1071), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%310), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %705
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1072: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%311, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1072), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%311), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %706
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1073: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%312, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1073), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%312), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %707
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1074: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%313, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1074), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%313), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %708
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1075: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%314, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1075), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%314), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %709
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1076: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%315, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%1076), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%315), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %710
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1077: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%316, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%1077), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<f64>(1.5)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%316), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %711
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1078: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%317, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%1078), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%317), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %712
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %713
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1079: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%318, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%1079)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%318)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %714
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1080: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%319, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1080)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%319)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %715
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1081: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%320, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1081)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%320)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %716
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1082: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%321, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1082))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%321))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %717
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1083: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%322, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1083)), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%322)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %718
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1084: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%323, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1084))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%323))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %719
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1085: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%324, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%1085), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), neg<i32, overflow=ub>(const<i32>(1))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%324), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %720
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1086: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%325, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%1086), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%325), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %721
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1087: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%326, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1087), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%326), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %722
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1088: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%327, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1088), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%327), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %723
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1089: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%328, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1089), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%328), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %724
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1090: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%329, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1090), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%329), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %725
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1091: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%330, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%1091), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%330), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %726
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1092: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%331, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%1092), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), const<f64>(1.5)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%331), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %727
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1093: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%332, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%1093), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%332), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %728
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %729
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1094: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%333, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%1094)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%333)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %730
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1095: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%334, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1095)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%334)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %731
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1096: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%335, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1096)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%335)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %732
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1097: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%336, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1097))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%336))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %733
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1098: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%337, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1098)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%337)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %734
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1099: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%338, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1099))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%338))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %735
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1100: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%339, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%1100), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), const<i32>(1)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%339), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %736
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1101: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%340, add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%1101), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%340), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %737
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1102: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%341, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1102), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%341), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %738
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1103: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%342, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1103), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%342), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %739
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1104: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%343, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1104), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%343), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %740
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1105: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%344, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1105), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%344), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %741
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1106: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%345, add<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%1106), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%345), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %742
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1107: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%346, add<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%1107), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<f64>(1.5)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%346), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %743
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1108: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%347, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%1108), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%347), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %744
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %745
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1109: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%348, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%1109)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%348)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %746
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1110: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%349, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1110)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%349)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %747
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1111: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%350, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1111)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%350)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %748
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1112: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%351, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1112))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%351))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %749
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1113: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%352, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1113)), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%352)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %750
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1114: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%353, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1114))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%353))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %751
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1115: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%354, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%1115), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), neg<i32, overflow=ub>(const<i32>(1))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%354), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %752
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1116: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%355, sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%1116), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%355), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %753
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1117: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%356, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1117), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%356), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %754
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1118: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%357, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1118), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%357), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %755
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1119: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%358, sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%1119), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%358), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %756
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1120: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%359, sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%1120), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%359), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %757
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1121: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%360, sub<f32, rounding=nearest_even, exceptions=observable, contract=off>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32>(%1121), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%360), add<f32, rounding=nearest_even, exceptions=observable, contract=off>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %758
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1122: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%361, sub<f64, rounding=nearest_even, exceptions=observable, contract=off>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64>(%1122), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), const<f64>(1.5)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%361), add<f64, rounding=nearest_even, exceptions=observable, contract=off>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %759
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %1123: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%362, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80>(%1123), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))), float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%362), add<f80, rounding=nearest_even, exceptions=observable, contract=off>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %760
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %1124: ptr<i32> [synthetic] = update<ptr<i32>, result=new, volatile, atomic=seq_cst>(%364, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>>(%1124), conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))), const<i32>(1)), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%364), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %761
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %1125: ptr<i32> [synthetic] = update<ptr<i32>, result=new, volatile, atomic=seq_cst>(%365, ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>>(%1125), conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%365), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %762
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %1126: ptr<i32> [synthetic] = update<ptr<i32>, result=old, volatile, atomic=seq_cst>(%366, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>>(%1126), conditional<ptr<i32>>(ne<i32>(const<i32>(0), const<i32>(0)), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))), const<i32>(1)), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%366), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %763
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %1127: ptr<i32> [synthetic] = update<ptr<i32>, result=old, volatile, atomic=seq_cst>(%367, ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>>(%1127), conditional<ptr<i32>>(ne<i32>(const<i32>(0), const<i32>(0)), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%367), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%363), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %368 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
