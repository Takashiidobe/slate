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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 a: array<i16, 1024>;
// DEFAULT-NEXT:     } [size=2048, align=2, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_2:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_2:[0-9]+]] b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_3:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_3:[0-9]+]] b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_4:[0-9]+]] a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_4:[0-9]+]] b: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_5:[0-9]+]] a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_5:[0-9]+]] b: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_6:[0-9]+]] a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_6:[0-9]+]] b: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_7:[0-9]+]] a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_7:[0-9]+]] b: volatile atomic i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_8:[0-9]+]] a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_8:[0-9]+]] b: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_9:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_9:[0-9]+]] b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_10:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_10:[0-9]+]] b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_11:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_11:[0-9]+]] b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_12:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_12:[0-9]+]] b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_13:[0-9]+]] a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_13:[0-9]+]] b: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_14:[0-9]+]] a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_14:[0-9]+]] b: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_15:[0-9]+]] a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_15:[0-9]+]] b: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_16:[0-9]+]] a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_16:[0-9]+]] b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_17:[0-9]+]] a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_17:[0-9]+]] b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_18:[0-9]+]] a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_18:[0-9]+]] b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_19:[0-9]+]] a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_19:[0-9]+]] b: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_20:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_20:[0-9]+]] b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_21:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_21:[0-9]+]] b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_22:[0-9]+]] a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_22:[0-9]+]] b: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_23:[0-9]+]] a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_23:[0-9]+]] b: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_24:[0-9]+]] a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_24:[0-9]+]] b: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_25:[0-9]+]] a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_25:[0-9]+]] b: volatile atomic i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_26:[0-9]+]] a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_26:[0-9]+]] b: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_27:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_27:[0-9]+]] b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_28:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_28:[0-9]+]] b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_29:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_29:[0-9]+]] b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_30:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_30:[0-9]+]] b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_31:[0-9]+]] a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_31:[0-9]+]] b: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_32:[0-9]+]] a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_32:[0-9]+]] b: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_33:[0-9]+]] a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_33:[0-9]+]] b: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_34:[0-9]+]] a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_34:[0-9]+]] b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_35:[0-9]+]] a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_35:[0-9]+]] b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_36:[0-9]+]] a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_36:[0-9]+]] b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_37:[0-9]+]] a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_37:[0-9]+]] b: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_38:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_38:[0-9]+]] b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_39:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_39:[0-9]+]] b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_40:[0-9]+]] a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_40:[0-9]+]] b: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_41:[0-9]+]] a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_41:[0-9]+]] b: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_42:[0-9]+]] a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_42:[0-9]+]] b: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_43:[0-9]+]] a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_43:[0-9]+]] b: volatile atomic i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_44:[0-9]+]] a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_44:[0-9]+]] b: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_45:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_45:[0-9]+]] b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_46:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_46:[0-9]+]] b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_47:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_47:[0-9]+]] b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_48:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_48:[0-9]+]] b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_49:[0-9]+]] a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_49:[0-9]+]] b: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_50:[0-9]+]] a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_50:[0-9]+]] b: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_51:[0-9]+]] a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_51:[0-9]+]] b: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_52:[0-9]+]] a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_52:[0-9]+]] b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_53:[0-9]+]] a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_53:[0-9]+]] b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_54:[0-9]+]] a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_54:[0-9]+]] b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_55:[0-9]+]] a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_55:[0-9]+]] b: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_56:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_56:[0-9]+]] b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_57:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_57:[0-9]+]] b: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_58:[0-9]+]] a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_58:[0-9]+]] b: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_59:[0-9]+]] a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_59:[0-9]+]] b: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_60:[0-9]+]] a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_60:[0-9]+]] b: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_61:[0-9]+]] a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_61:[0-9]+]] b: volatile atomic i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_62:[0-9]+]] a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_62:[0-9]+]] b: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_63:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_63:[0-9]+]] b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_64:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_64:[0-9]+]] b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_65:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_65:[0-9]+]] b: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_66:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_66:[0-9]+]] b: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_67:[0-9]+]] a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_67:[0-9]+]] b: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_68:[0-9]+]] a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_68:[0-9]+]] b: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_69:[0-9]+]] a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_69:[0-9]+]] b: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_70:[0-9]+]] a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_70:[0-9]+]] b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_71:[0-9]+]] a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_71:[0-9]+]] b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_72:[0-9]+]] a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_72:[0-9]+]] b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_73:[0-9]+]] a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_73:[0-9]+]] b: volatile atomic bool [storage=static] = ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)), const<u64>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_74:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_74:[0-9]+]] b: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_75:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_75:[0-9]+]] b: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_76:[0-9]+]] a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_76:[0-9]+]] b: volatile atomic u8 [storage=static] = truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_77:[0-9]+]] a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_77:[0-9]+]] b: volatile atomic i16 [storage=static] = reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_78:[0-9]+]] a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_78:[0-9]+]] b: volatile atomic u16 [storage=static] = truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_79:[0-9]+]] a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_79:[0-9]+]] b: volatile atomic i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_80:[0-9]+]] a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_80:[0-9]+]] b: volatile atomic u32 [storage=static] = truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_81:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_81:[0-9]+]] b: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_82:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_82:[0-9]+]] b: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_83:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_83:[0-9]+]] b: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_84:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_84:[0-9]+]] b: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_85:[0-9]+]] a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_85:[0-9]+]] b: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_86:[0-9]+]] a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_86:[0-9]+]] b: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_87:[0-9]+]] a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_87:[0-9]+]] b: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_88:[0-9]+]] a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_88:[0-9]+]] b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_89:[0-9]+]] a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_89:[0-9]+]] b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_90:[0-9]+]] a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_90:[0-9]+]] b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_91:[0-9]+]] a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_91:[0-9]+]] b: volatile atomic bool [storage=static] = ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_92:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_92:[0-9]+]] b: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_93:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_93:[0-9]+]] b: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_94:[0-9]+]] a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_94:[0-9]+]] b: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_95:[0-9]+]] a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_95:[0-9]+]] b: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_96:[0-9]+]] a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_96:[0-9]+]] b: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_97:[0-9]+]] a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_97:[0-9]+]] b: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_98:[0-9]+]] a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_98:[0-9]+]] b: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_99:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_99:[0-9]+]] b: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_100:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_100:[0-9]+]] b: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_101:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_101:[0-9]+]] b: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_102:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_102:[0-9]+]] b: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_103:[0-9]+]] a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_103:[0-9]+]] b: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_104:[0-9]+]] a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_104:[0-9]+]] b: volatile atomic f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_105:[0-9]+]] a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_105:[0-9]+]] b: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_106:[0-9]+]] a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_106:[0-9]+]] b: volatile atomic complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=explicit>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_107:[0-9]+]] a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_107:[0-9]+]] b: volatile atomic complex<f64> [storage=static] = real_to_complex<complex<f64>, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_108:[0-9]+]] a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_108:[0-9]+]] b: volatile atomic complex<f80> [storage=static] = real_to_complex<complex<f80>, reason=explicit>(float_widen<f80, reason=explicit>(const<f64>(1.5))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_109:[0-9]+]] a: volatile atomic bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_109:[0-9]+]] b: volatile atomic bool [storage=static] = ne<complex<f64>, reason=explicit, exceptions=ignore>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_110:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_110:[0-9]+]] b: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_111:[0-9]+]] a: volatile atomic i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_111:[0-9]+]] b: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_112:[0-9]+]] a: volatile atomic u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_112:[0-9]+]] b: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_113:[0-9]+]] a: volatile atomic i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_113:[0-9]+]] b: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_114:[0-9]+]] a: volatile atomic u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_114:[0-9]+]] b: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_115:[0-9]+]] a: volatile atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_115:[0-9]+]] b: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_116:[0-9]+]] a: volatile atomic u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_116:[0-9]+]] b: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_117:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_117:[0-9]+]] b: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_118:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_118:[0-9]+]] b: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_119:[0-9]+]] a: volatile atomic i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_119:[0-9]+]] b: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_120:[0-9]+]] a: volatile atomic u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_120:[0-9]+]] b: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_121:[0-9]+]] a: volatile atomic f32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_121:[0-9]+]] b: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_122:[0-9]+]] a: volatile atomic f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_122:[0-9]+]] b: volatile atomic f64 [storage=static] = complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_123:[0-9]+]] a: volatile atomic f80 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_123:[0-9]+]] b: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_124:[0-9]+]] a: volatile atomic complex<f32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_124:[0-9]+]] b: volatile atomic complex<f32> [storage=static] = complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=ignore>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_125:[0-9]+]] a: volatile atomic complex<f64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_125:[0-9]+]] b: volatile atomic complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_126:[0-9]+]] a: volatile atomic complex<f80> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_126:[0-9]+]] b: volatile atomic complex<f80> [storage=static] = complex_convert<complex<f80>, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_127:[0-9]+]] a: volatile atomic ptr<i32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_127:[0-9]+]] b: volatile atomic ptr<i32> [storage=static] = null<ptr<i32>> [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_128:[0-9]+]] a: volatile atomic ptr<i32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b_128:[0-9]+]] b: volatile atomic ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_i]]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_simple_assign:[0-9]+]] @test_simple_assign() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a]], read<bool, volatile, atomic=seq_cst>(%[[VALUE_b]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_2]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_2]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_2]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_2]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_2]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_3]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_3]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_3]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_3]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_3]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_4]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_4]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_4]], read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_4]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_4]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_5]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_5]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_5]], read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_5]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_5]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_5]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_6]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_6]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_6]], read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_6]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_6]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_6]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_7]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_7]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_7]], read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_7]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_7]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_7]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_8]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_8]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_8]], read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_8]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_8]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_8]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_9]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_9]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_9]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_9]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_9]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_9]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_10]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_10]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_10]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_10]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_10]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_10]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_11]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_11]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_11]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_11]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_11]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_11]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_12]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_12]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_12]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_12]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_12]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_12]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_13]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_13]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_13]], read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_13]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_13]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_13]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_14]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_14]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_14]], read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_14]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_14]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_14]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_15]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_15]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_15]], read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_15]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_15]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_15]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_16]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_16]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_16]], read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_16]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_16]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_16]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_17]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_17]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_17]], read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_17]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_17]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_17]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_18]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_18]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_18]], read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_18]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_18]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_18]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_19]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_19]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_19]], read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_19]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_19]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_19]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_20]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_20]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_20]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_20]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_20]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_20]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_21]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_21]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_21]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_21]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_21]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_21]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_22]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_22]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_22]], read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_22]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_22]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_22]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_23]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_23]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_23]], read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_23]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_23]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_23]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_24]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_24]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_24]], read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_24]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_24]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_24]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_25]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_25]]), const<i32>(1))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_25]], read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_25]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_25]]), const<i32>(1))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_25]]), const<i32>(1))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_26]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_26]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_26]], read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_26]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_26]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_26]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_27]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_27]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_27]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_27]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_27]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_27]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_28]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_28]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_28]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_28]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_28]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_28]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_29]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_29]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_29]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_29]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_29]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_29]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_30]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_30]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_30]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_30]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_30]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_30]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_31]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_31]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_31]], read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_31]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_31]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_31]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_32]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_32]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_32]], read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_32]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_32]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_32]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE38:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_33]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_33]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_33]], read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_33]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_33]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_33]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE39:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_34]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_34]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_34]], read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_34]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_34]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_34]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE40:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_35]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_35]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_35]], read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_35]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_35]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_35]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE41:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_36]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_36]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_36]], read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_36]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_36]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_36]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE43:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_37]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_37]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_37]], read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_37]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_37]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_37]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE44:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_38]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_38]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_38]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_38]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_38]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_38]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE45:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_39]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_39]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_39]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_39]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_39]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_39]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE46:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_40]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_40]], read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_40]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE47:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_41]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_41]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_41]], read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_41]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_41]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_41]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE48:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_42]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_42]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_42]], read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_42]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_42]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_42]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE49:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_43]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_43]]), const<i32>(2))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_43]], read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_43]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_43]]), const<i32>(2))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_43]]), const<i32>(2))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE50:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_44]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_44]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_44]], read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_44]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_44]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_44]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE51:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_45]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_45]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_45]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_45]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_45]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_45]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_46]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_46]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_46]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_46]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_46]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_46]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE53:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_47]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_47]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_47]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_47]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_47]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_47]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE54:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_48]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_48]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_48]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_48]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_48]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_48]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE55:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_49]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_49]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_49]], read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_49]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_49]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_49]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE56:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_50]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_50]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_50]], read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_50]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_50]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_50]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE57:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_51]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_51]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_51]], read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_51]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_51]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_51]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE58:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_52]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_52]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_52]], read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_52]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_52]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_52]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE59:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_53]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_53]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_53]], read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_53]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_53]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_53]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE60:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_54]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_54]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_54]], read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_54]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_54]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_54]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE61:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE62:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_55]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_55]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_55]], read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_55]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_55]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_55]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE63:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_56]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_56]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_56]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_56]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_56]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_56]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE64:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_57]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_57]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_57]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_57]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_57]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_57]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE65:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_58]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_58]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_58]], read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_58]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_58]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_58]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE66:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_59]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_59]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_59]], read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_59]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_59]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_59]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE67:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_60]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_60]], read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_60]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE68:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_61]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_61]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_61]], read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_61]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_61]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_61]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE69:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_62]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_62]]), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_62]], read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_62]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_62]]), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_62]]), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE70:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_63]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_63]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_63]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_63]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_63]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_63]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE71:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_64]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_64]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_64]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_64]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_64]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_64]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE72:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_65]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_65]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_65]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_65]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_65]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_65]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE73:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_66]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_66]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_66]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_66]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_66]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_66]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE74:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_67]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_67]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_67]], read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_67]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_67]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_67]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE75:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_68]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_68]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_68]], read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_68]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_68]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_68]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE76:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_69]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_69]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_69]], read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_69]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_69]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_69]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE77:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_70]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_70]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_70]], read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_70]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_70]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_70]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE78:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_71]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_71]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_71]], read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_71]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_71]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_71]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE79:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_72]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_72]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_72]], read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_72]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_72]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_72]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE80:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE81:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_73]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_73]])), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)), const<u64>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_73]], read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_73]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_73]])), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)), const<u64>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_73]])), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)), const<u64>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE82:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_74]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_74]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_74]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_74]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_74]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_74]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE83:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_75]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_75]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_75]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_75]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_75]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_75]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE84:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_76]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_76]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_76]], read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_76]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_76]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_76]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE85:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_77]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_77]])), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_77]], read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_77]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_77]])), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_77]])), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE86:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_78]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_78]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_78]], read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_78]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_78]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_78]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE87:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_79]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_79]]), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_79]], read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_79]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_79]]), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_79]]), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE88:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_80]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_80]]), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_80]], read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_80]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_80]]), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_80]]), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE89:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_81]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_81]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_81]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_81]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_81]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_81]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE90:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_82]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_82]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_82]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_82]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_82]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_82]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE91:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_83]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_83]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_83]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_83]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_83]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_83]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE92:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_84]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_84]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_84]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_84]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_84]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_84]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE93:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_85]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_85]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_85]], read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_85]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_85]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_85]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE94:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_86]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_86]]), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_86]], read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_86]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_86]]), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_86]]), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE95:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_87]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_87]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_87]], read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_87]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_87]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_87]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE96:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_88]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_88]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_88]], read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_88]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_88]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_88]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE97:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_89]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_89]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_89]], read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_89]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_89]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_89]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE98:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_90]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_90]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_90]], read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_90]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_90]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_90]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE99:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE100:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_91]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_91]])), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_91]], read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_91]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_91]])), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_91]])), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE101:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_92]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_92]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_92]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_92]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_92]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_92]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE102:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_93]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_93]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_93]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_93]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_93]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_93]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE103:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_94]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_94]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_94]], read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_94]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_94]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_94]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE104:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_95]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_95]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_95]], read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_95]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_95]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_95]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE105:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_96]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_96]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_96]], read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_96]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_96]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_96]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE106:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_97]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_97]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_97]], read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_97]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_97]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_97]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE107:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_98]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_98]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_98]], read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_98]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_98]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_98]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE108:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_99]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_99]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_99]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_99]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_99]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_99]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE109:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_100]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_100]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_100]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_100]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_100]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_100]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE110:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_101]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_101]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_101]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_101]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_101]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_101]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE111:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_102]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_102]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_102]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_102]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_102]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_102]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE112:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_103]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_103]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_103]], read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_103]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_103]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_103]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE113:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_104]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_104]]), const<f64>(1.5))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_104]], read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_104]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_104]]), const<f64>(1.5))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_104]]), const<f64>(1.5))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE114:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_105]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_105]]), float_widen<f80, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_105]], read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_105]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_105]]), float_widen<f80, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_105]]), float_widen<f80, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE115:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_106]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_106]]), real_to_complex<complex<f32>, reason=explicit>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_106]], read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_106]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_106]]), real_to_complex<complex<f32>, reason=explicit>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_106]]), real_to_complex<complex<f32>, reason=explicit>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE116:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_107]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_107]]), real_to_complex<complex<f64>, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_107]], read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_107]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_107]]), real_to_complex<complex<f64>, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_107]]), real_to_complex<complex<f64>, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE117:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_108]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_108]]), real_to_complex<complex<f80>, reason=explicit>(float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_108]], read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_108]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_108]]), real_to_complex<complex<f80>, reason=explicit>(float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_108]]), real_to_complex<complex<f80>, reason=explicit>(float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE118:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE119:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_109]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_109]])), from_bool<i32, reason=promotion>(ne<complex<f64>, reason=explicit, exceptions=ignore>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_109]], read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_109]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_109]])), from_bool<i32, reason=promotion>(ne<complex<f64>, reason=explicit, exceptions=ignore>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_109]])), from_bool<i32, reason=promotion>(ne<complex<f64>, reason=explicit, exceptions=ignore>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE120:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_110]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_110]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_110]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_110]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_110]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_110]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE121:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_111]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_111]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_111]], read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_111]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_111]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_111]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE122:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_112]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_112]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_112]], read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_112]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_112]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_112]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE123:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_113]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_113]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_113]], read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_113]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_113]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_113]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE124:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_114]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_114]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_114]], read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_114]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_114]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_114]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE125:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_115]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_115]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_115]], read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_115]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_115]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_115]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE126:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_116]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_116]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_116]], read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_116]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_116]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_116]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE127:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_117]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_117]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_117]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_117]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_117]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_117]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE128:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_118]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_118]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_118]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_118]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_118]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_118]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE129:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_119]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_119]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_119]], read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_119]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_119]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_119]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE130:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_120]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_120]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_120]], read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_120]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_120]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_120]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE131:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_121]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_121]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_121]], read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_121]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_121]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_121]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE132:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_122]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_122]]), complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_122]], read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_122]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_122]]), complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_122]]), complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE133:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_123]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_123]]), float_widen<f80, reason=explicit>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_123]], read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_123]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_123]]), float_widen<f80, reason=explicit>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_123]]), float_widen<f80, reason=explicit>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE134:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_124]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_124]]), complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=ignore>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_124]], read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_124]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_124]]), complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=ignore>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_124]]), complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=ignore>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE135:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_125]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_125]]), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_125]], read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_125]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_125]]), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_125]]), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE136:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_126]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_126]]), complex_convert<complex<f80>, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_126]], read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_126]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_126]]), complex_convert<complex<f80>, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_126]]), complex_convert<complex<f80>, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE137:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_127]]), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_b_127]]), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_127]], read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_b_127]]));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_b_127]]), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_127]]), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE138:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_128]]), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_b_128]]), addr_of<ptr<i32>>(%[[VALUE_i]]))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_128]], read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_b_128]]));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_b_128]]), addr_of<ptr<i32>>(%[[VALUE_i]]))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_128]]), addr_of<ptr<i32>>(%[[VALUE_i]]))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_init:[0-9]+]] init: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_copy:[0-9]+]] copy: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s1:[0-9]+]] s1: atomic @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s2:[0-9]+]] s2: atomic @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE139:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE140:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE141:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE140]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE141]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(field0(%[[VALUE_init]])), read<i32>(%[[VALUE_j]]))), truncate<i16, reason=assign, fits=unknown>(read<i32>(%[[VALUE_j]])));
// DEFAULT-NEXT:         write<@type[[TYPE_s]], atomic=seq_cst>(%[[VALUE_s1]], copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]]>(%[[VALUE_init]])));
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_copy]], copy<@type[[TYPE_s]], reason=assign>(copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]]>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_init]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_copy]])), const<u64>(2048)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]], atomic=seq_cst>(%[[VALUE_s2]], copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]], atomic=seq_cst>(%[[VALUE_s1]])));
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_copy]], copy<@type[[TYPE_s]], reason=assign>(copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]], atomic=seq_cst>(%[[VALUE_s1]]))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_init]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_copy]])), const<u64>(2048)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_copy]], copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]], atomic=seq_cst>(%[[VALUE_s1]])));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_init]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_copy]])), const<u64>(2048)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_copy]], copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]], atomic=seq_cst>(%[[VALUE_s2]])));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_init]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_copy]])), const<u64>(2048)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_simple_assign]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
