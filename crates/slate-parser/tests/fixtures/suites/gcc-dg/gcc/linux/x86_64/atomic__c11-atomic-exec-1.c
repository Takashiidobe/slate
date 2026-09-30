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
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: bool [synthetic] = read<bool, volatile, atomic=seq_cst>(%[[VALUE_b]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a]], read<bool>(%[[VALUE6]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE6]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_2]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_2]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE8:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_2]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_2]], read<i8>(%[[VALUE8]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE8]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_2]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_3]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_3]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_3]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_3]], read<i8>(%[[VALUE10]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE10]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_3]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_4]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_4]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE12:[0-9]+]]: u8 [synthetic] = read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_4]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_4]], read<u8>(%[[VALUE12]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE12]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_4]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_5]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_5]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE14:[0-9]+]]: i16 [synthetic] = read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_5]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_5]], read<i16>(%[[VALUE14]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE14]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_5]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_6]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_6]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE16:[0-9]+]]: u16 [synthetic] = read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_6]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_6]], read<u16>(%[[VALUE16]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE16]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_6]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_7]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_7]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_7]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_7]], read<i32>(%[[VALUE18]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%[[VALUE18]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_7]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_8]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_8]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE20:[0-9]+]]: u32 [synthetic] = read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_8]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_8]], read<u32>(%[[VALUE20]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32>(%[[VALUE20]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_8]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_9]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_9]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE22:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_9]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_9]], read<i64>(%[[VALUE22]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE22]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_9]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_10]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_10]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE24:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_10]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_10]], read<u64>(%[[VALUE24]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE24]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_10]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_11]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_11]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE26:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_11]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_11]], read<i64>(%[[VALUE26]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE26]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_11]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_12]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_12]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE28:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_12]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_12]], read<u64>(%[[VALUE28]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE28]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_12]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_13]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_13]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE30:[0-9]+]]: f32 [synthetic] = read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_13]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_13]], read<f32>(%[[VALUE30]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE30]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_13]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_14]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_14]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE32:[0-9]+]]: f64 [synthetic] = read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_14]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_14]], read<f64>(%[[VALUE32]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE32]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_14]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_15]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_15]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE34:[0-9]+]]: f80 [synthetic] = read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_15]], read<f80>(%[[VALUE34]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80>(%[[VALUE34]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_15]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_16]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_16]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE36:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_16]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_16]], read<complex<f32>>(%[[VALUE36]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE36]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_16]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_17]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_17]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE38:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_17]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_17]], read<complex<f64>>(%[[VALUE38]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE38]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_17]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE39:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_18]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_18]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE40:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_18]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_18]], read<complex<f80>>(%[[VALUE40]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE40]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_18]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE41:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_19]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_19]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE43:[0-9]+]]: bool [synthetic] = read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_19]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_19]], read<bool>(%[[VALUE43]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE43]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_19]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE44:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_20]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_20]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE45:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_20]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_20]], read<i8>(%[[VALUE45]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE45]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_20]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE46:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_21]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_21]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE47:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_21]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_21]], read<i8>(%[[VALUE47]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE47]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_21]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE48:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_22]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_22]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE49:[0-9]+]]: u8 [synthetic] = read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_22]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_22]], read<u8>(%[[VALUE49]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE49]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_22]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE50:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_23]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_23]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE51:[0-9]+]]: i16 [synthetic] = read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_23]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_23]], read<i16>(%[[VALUE51]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE51]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_23]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_24]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_24]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE53:[0-9]+]]: u16 [synthetic] = read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_24]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_24]], read<u16>(%[[VALUE53]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE53]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_24]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE54:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_25]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_25]]), const<i32>(1))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE55:[0-9]+]]: i32 [synthetic] = read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_25]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_25]], read<i32>(%[[VALUE55]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%[[VALUE55]]), const<i32>(1))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_25]]), const<i32>(1))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE56:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_26]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_26]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE57:[0-9]+]]: u32 [synthetic] = read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_26]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_26]], read<u32>(%[[VALUE57]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32>(%[[VALUE57]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_26]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE58:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_27]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_27]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE59:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_27]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_27]], read<i64>(%[[VALUE59]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE59]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_27]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE60:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_28]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_28]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE61:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_28]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_28]], read<u64>(%[[VALUE61]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE61]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_28]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE62:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_29]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_29]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE63:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_29]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_29]], read<i64>(%[[VALUE63]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE63]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_29]]), widen<i64, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE64:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_30]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_30]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE65:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_30]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_30]], read<u64>(%[[VALUE65]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE65]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_30]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE66:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_31]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_31]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE67:[0-9]+]]: f32 [synthetic] = read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_31]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_31]], read<f32>(%[[VALUE67]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE67]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_31]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE68:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_32]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_32]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE69:[0-9]+]]: f64 [synthetic] = read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_32]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_32]], read<f64>(%[[VALUE69]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE69]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_32]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE70:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_33]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_33]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE71:[0-9]+]]: f80 [synthetic] = read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_33]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_33]], read<f80>(%[[VALUE71]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80>(%[[VALUE71]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_33]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE72:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_34]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_34]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE73:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_34]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_34]], read<complex<f32>>(%[[VALUE73]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE73]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_34]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE74:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_35]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_35]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE75:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_35]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_35]], read<complex<f64>>(%[[VALUE75]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE75]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_35]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE76:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_36]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_36]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE77:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_36]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_36]], read<complex<f80>>(%[[VALUE77]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE77]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_36]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE78:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE79:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_37]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_37]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE80:[0-9]+]]: bool [synthetic] = read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_37]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_37]], read<bool>(%[[VALUE80]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE80]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_37]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE81:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_38]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_38]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE82:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_38]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_38]], read<i8>(%[[VALUE82]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE82]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_38]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE83:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_39]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_39]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE84:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_39]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_39]], read<i8>(%[[VALUE84]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE84]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_39]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE85:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_40]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE86:[0-9]+]]: u8 [synthetic] = read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_40]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_40]], read<u8>(%[[VALUE86]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE86]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE87:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_41]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_41]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE88:[0-9]+]]: i16 [synthetic] = read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_41]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_41]], read<i16>(%[[VALUE88]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE88]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_41]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE89:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_42]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_42]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE90:[0-9]+]]: u16 [synthetic] = read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_42]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_42]], read<u16>(%[[VALUE90]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE90]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_42]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE91:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_43]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_43]]), const<i32>(2))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE92:[0-9]+]]: i32 [synthetic] = read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_43]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_43]], read<i32>(%[[VALUE92]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%[[VALUE92]]), const<i32>(2))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_43]]), const<i32>(2))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE93:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_44]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_44]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE94:[0-9]+]]: u32 [synthetic] = read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_44]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_44]], read<u32>(%[[VALUE94]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32>(%[[VALUE94]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_44]]), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE95:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_45]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_45]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE96:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_45]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_45]], read<i64>(%[[VALUE96]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE96]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_45]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE97:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_46]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_46]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE98:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_46]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_46]], read<u64>(%[[VALUE98]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE98]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_46]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE99:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_47]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_47]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE100:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_47]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_47]], read<i64>(%[[VALUE100]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE100]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_47]]), widen<i64, reason=explicit>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE101:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_48]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_48]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE102:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_48]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_48]], read<u64>(%[[VALUE102]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE102]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_48]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE103:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_49]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_49]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE104:[0-9]+]]: f32 [synthetic] = read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_49]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_49]], read<f32>(%[[VALUE104]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE104]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_49]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE105:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_50]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_50]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE106:[0-9]+]]: f64 [synthetic] = read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_50]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_50]], read<f64>(%[[VALUE106]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE106]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_50]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE107:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_51]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_51]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE108:[0-9]+]]: f80 [synthetic] = read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_51]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_51]], read<f80>(%[[VALUE108]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80>(%[[VALUE108]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_51]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE109:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_52]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_52]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE110:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_52]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_52]], read<complex<f32>>(%[[VALUE110]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE110]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_52]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE111:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_53]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_53]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE112:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_53]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_53]], read<complex<f64>>(%[[VALUE112]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE112]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_53]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE113:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_54]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_54]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE114:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_54]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_54]], read<complex<f80>>(%[[VALUE114]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE114]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_54]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE115:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE116:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_55]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_55]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE117:[0-9]+]]: bool [synthetic] = read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_55]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_55]], read<bool>(%[[VALUE117]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE117]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_55]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE118:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_56]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_56]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE119:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_56]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_56]], read<i8>(%[[VALUE119]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE119]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_56]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE120:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_57]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_57]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE121:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_57]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_57]], read<i8>(%[[VALUE121]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE121]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_57]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE122:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_58]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_58]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE123:[0-9]+]]: u8 [synthetic] = read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_58]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_58]], read<u8>(%[[VALUE123]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE123]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_58]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE124:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_59]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_59]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE125:[0-9]+]]: i16 [synthetic] = read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_59]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_59]], read<i16>(%[[VALUE125]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE125]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_59]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE126:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_60]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE127:[0-9]+]]: u16 [synthetic] = read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_60]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_60]], read<u16>(%[[VALUE127]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE127]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE128:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_61]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_61]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE129:[0-9]+]]: i32 [synthetic] = read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_61]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_61]], read<i32>(%[[VALUE129]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%[[VALUE129]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_61]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE130:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_62]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_62]]), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE131:[0-9]+]]: u32 [synthetic] = read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_62]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_62]], read<u32>(%[[VALUE131]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32>(%[[VALUE131]]), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_62]]), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE132:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_63]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_63]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE133:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_63]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_63]], read<i64>(%[[VALUE133]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE133]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_63]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE134:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_64]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_64]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE135:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_64]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_64]], read<u64>(%[[VALUE135]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE135]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_64]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE136:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_65]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_65]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE137:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_65]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_65]], read<i64>(%[[VALUE137]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE137]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_65]]), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE138:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_66]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_66]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE139:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_66]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_66]], read<u64>(%[[VALUE139]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE139]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_66]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE140:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_67]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_67]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE141:[0-9]+]]: f32 [synthetic] = read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_67]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_67]], read<f32>(%[[VALUE141]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE141]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_67]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE142:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_68]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_68]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE143:[0-9]+]]: f64 [synthetic] = read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_68]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_68]], read<f64>(%[[VALUE143]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE143]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_68]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE144:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_69]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_69]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE145:[0-9]+]]: f80 [synthetic] = read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_69]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_69]], read<f80>(%[[VALUE145]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80>(%[[VALUE145]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_69]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE146:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_70]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_70]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE147:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_70]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_70]], read<complex<f32>>(%[[VALUE147]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE147]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_70]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE148:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_71]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_71]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE149:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_71]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_71]], read<complex<f64>>(%[[VALUE149]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE149]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_71]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE150:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_72]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_72]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE151:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_72]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_72]], read<complex<f80>>(%[[VALUE151]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE151]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_72]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE152:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE153:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_73]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_73]])), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)), const<u64>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE154:[0-9]+]]: bool [synthetic] = read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_73]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_73]], read<bool>(%[[VALUE154]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE154]])), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)), const<u64>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_73]])), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)), const<u64>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE155:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_74]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_74]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE156:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_74]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_74]], read<i8>(%[[VALUE156]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE156]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_74]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE157:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_75]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_75]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE158:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_75]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_75]], read<i8>(%[[VALUE158]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE158]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_75]])), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE159:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_76]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_76]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE160:[0-9]+]]: u8 [synthetic] = read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_76]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_76]], read<u8>(%[[VALUE160]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE160]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_76]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE161:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_77]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_77]])), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE162:[0-9]+]]: i16 [synthetic] = read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_77]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_77]], read<i16>(%[[VALUE162]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE162]])), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_77]])), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE163:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_78]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_78]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE164:[0-9]+]]: u16 [synthetic] = read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_78]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_78]], read<u16>(%[[VALUE164]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE164]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_78]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE165:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_79]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_79]]), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE166:[0-9]+]]: i32 [synthetic] = read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_79]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_79]], read<i32>(%[[VALUE166]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%[[VALUE166]]), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_79]]), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE167:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_80]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_80]]), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE168:[0-9]+]]: u32 [synthetic] = read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_80]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_80]], read<u32>(%[[VALUE168]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32>(%[[VALUE168]]), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_80]]), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE169:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_81]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_81]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE170:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_81]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_81]], read<i64>(%[[VALUE170]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE170]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_81]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE171:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_82]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_82]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE172:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_82]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_82]], read<u64>(%[[VALUE172]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE172]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_82]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE173:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_83]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_83]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE174:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_83]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_83]], read<i64>(%[[VALUE174]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE174]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_83]]), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE175:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_84]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_84]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE176:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_84]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_84]], read<u64>(%[[VALUE176]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE176]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_84]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE177:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_85]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_85]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE178:[0-9]+]]: f32 [synthetic] = read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_85]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_85]], read<f32>(%[[VALUE178]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE178]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_85]]), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE179:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_86]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_86]]), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE180:[0-9]+]]: f64 [synthetic] = read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_86]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_86]], read<f64>(%[[VALUE180]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE180]]), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_86]]), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE181:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_87]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_87]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE182:[0-9]+]]: f80 [synthetic] = read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_87]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_87]], read<f80>(%[[VALUE182]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80>(%[[VALUE182]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_87]]), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE183:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_88]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_88]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE184:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_88]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_88]], read<complex<f32>>(%[[VALUE184]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE184]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_88]]), real_to_complex<complex<f32>, reason=explicit>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE185:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_89]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_89]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE186:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_89]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_89]], read<complex<f64>>(%[[VALUE186]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE186]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_89]]), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE187:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_90]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_90]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE188:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_90]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_90]], read<complex<f80>>(%[[VALUE188]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE188]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_90]]), real_to_complex<complex<f80>, reason=explicit>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE189:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE190:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_91]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_91]])), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE191:[0-9]+]]: bool [synthetic] = read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_91]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_91]], read<bool>(%[[VALUE191]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE191]])), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_91]])), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(1.5), const<f64>(0.0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE192:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_92]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_92]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE193:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_92]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_92]], read<i8>(%[[VALUE193]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE193]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_92]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE194:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_93]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_93]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE195:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_93]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_93]], read<i8>(%[[VALUE195]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE195]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_93]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE196:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_94]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_94]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE197:[0-9]+]]: u8 [synthetic] = read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_94]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_94]], read<u8>(%[[VALUE197]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE197]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_94]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE198:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_95]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_95]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE199:[0-9]+]]: i16 [synthetic] = read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_95]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_95]], read<i16>(%[[VALUE199]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE199]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_95]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE200:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_96]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_96]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE201:[0-9]+]]: u16 [synthetic] = read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_96]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_96]], read<u16>(%[[VALUE201]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE201]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_96]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE202:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_97]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_97]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE203:[0-9]+]]: i32 [synthetic] = read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_97]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_97]], read<i32>(%[[VALUE203]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%[[VALUE203]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_97]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE204:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_98]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_98]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE205:[0-9]+]]: u32 [synthetic] = read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_98]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_98]], read<u32>(%[[VALUE205]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32>(%[[VALUE205]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_98]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE206:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_99]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_99]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE207:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_99]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_99]], read<i64>(%[[VALUE207]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE207]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_99]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE208:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_100]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_100]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE209:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_100]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_100]], read<u64>(%[[VALUE209]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE209]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_100]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE210:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_101]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_101]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE211:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_101]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_101]], read<i64>(%[[VALUE211]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE211]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_101]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE212:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_102]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_102]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE213:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_102]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_102]], read<u64>(%[[VALUE213]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE213]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_102]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE214:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_103]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_103]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE215:[0-9]+]]: f32 [synthetic] = read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_103]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_103]], read<f32>(%[[VALUE215]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE215]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_103]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE216:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_104]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_104]]), const<f64>(1.5))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE217:[0-9]+]]: f64 [synthetic] = read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_104]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_104]], read<f64>(%[[VALUE217]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE217]]), const<f64>(1.5))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_104]]), const<f64>(1.5))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE218:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_105]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_105]]), float_widen<f80, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE219:[0-9]+]]: f80 [synthetic] = read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_105]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_105]], read<f80>(%[[VALUE219]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80>(%[[VALUE219]]), float_widen<f80, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_105]]), float_widen<f80, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE220:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_106]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_106]]), real_to_complex<complex<f32>, reason=explicit>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE221:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_106]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_106]], read<complex<f32>>(%[[VALUE221]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE221]]), real_to_complex<complex<f32>, reason=explicit>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_106]]), real_to_complex<complex<f32>, reason=explicit>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE222:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_107]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_107]]), real_to_complex<complex<f64>, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE223:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_107]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_107]], read<complex<f64>>(%[[VALUE223]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE223]]), real_to_complex<complex<f64>, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_107]]), real_to_complex<complex<f64>, reason=explicit>(const<f64>(1.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE224:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_108]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_108]]), real_to_complex<complex<f80>, reason=explicit>(float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE225:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_108]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_108]], read<complex<f80>>(%[[VALUE225]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE225]]), real_to_complex<complex<f80>, reason=explicit>(float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_108]]), real_to_complex<complex<f80>, reason=explicit>(float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE226:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE227:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_109]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_109]])), from_bool<i32, reason=promotion>(ne<complex<f64>, reason=explicit, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE228:[0-9]+]]: bool [synthetic] = read<bool, volatile, atomic=seq_cst>(%[[VALUE_b_109]]);
// DEFAULT-NEXT:                         write<bool, volatile, atomic=seq_cst>(%[[VALUE_a_109]], read<bool>(%[[VALUE228]]));
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE228]])), from_bool<i32, reason=promotion>(ne<complex<f64>, reason=explicit, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_109]])), from_bool<i32, reason=promotion>(ne<complex<f64>, reason=explicit, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE229:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_110]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_110]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE230:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_110]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_110]], read<i8>(%[[VALUE230]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE230]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_110]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE231:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_111]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_111]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE232:[0-9]+]]: i8 [synthetic] = read<i8, volatile, atomic=seq_cst>(%[[VALUE_b_111]]);
// DEFAULT-NEXT:                         write<i8, volatile, atomic=seq_cst>(%[[VALUE_a_111]], read<i8>(%[[VALUE232]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE232]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_111]])), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE233:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_112]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_112]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE234:[0-9]+]]: u8 [synthetic] = read<u8, volatile, atomic=seq_cst>(%[[VALUE_b_112]]);
// DEFAULT-NEXT:                         write<u8, volatile, atomic=seq_cst>(%[[VALUE_a_112]], read<u8>(%[[VALUE234]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE234]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_112]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE235:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_113]])), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_113]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE236:[0-9]+]]: i16 [synthetic] = read<i16, volatile, atomic=seq_cst>(%[[VALUE_b_113]]);
// DEFAULT-NEXT:                         write<i16, volatile, atomic=seq_cst>(%[[VALUE_a_113]], read<i16>(%[[VALUE236]]));
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE236]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_113]])), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE237:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_114]]))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_114]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE238:[0-9]+]]: u16 [synthetic] = read<u16, volatile, atomic=seq_cst>(%[[VALUE_b_114]]);
// DEFAULT-NEXT:                         write<u16, volatile, atomic=seq_cst>(%[[VALUE_a_114]], read<u16>(%[[VALUE238]]));
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE238]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_114]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE239:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_115]]), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_115]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE240:[0-9]+]]: i32 [synthetic] = read<i32, volatile, atomic=seq_cst>(%[[VALUE_b_115]]);
// DEFAULT-NEXT:                         write<i32, volatile, atomic=seq_cst>(%[[VALUE_a_115]], read<i32>(%[[VALUE240]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%[[VALUE240]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_115]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE241:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_116]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_116]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE242:[0-9]+]]: u32 [synthetic] = read<u32, volatile, atomic=seq_cst>(%[[VALUE_b_116]]);
// DEFAULT-NEXT:                         write<u32, volatile, atomic=seq_cst>(%[[VALUE_a_116]], read<u32>(%[[VALUE242]]));
// DEFAULT-NEXT:                         if ne<u32>(read<u32>(%[[VALUE242]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_116]]), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE243:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_117]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_117]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE244:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_117]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_117]], read<i64>(%[[VALUE244]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE244]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_117]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE245:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_118]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_118]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE246:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_118]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_118]], read<u64>(%[[VALUE246]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE246]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_118]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE247:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_119]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_119]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE248:[0-9]+]]: i64 [synthetic] = read<i64, volatile, atomic=seq_cst>(%[[VALUE_b_119]]);
// DEFAULT-NEXT:                         write<i64, volatile, atomic=seq_cst>(%[[VALUE_a_119]], read<i64>(%[[VALUE248]]));
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE248]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_119]]), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE249:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_120]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_120]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE250:[0-9]+]]: u64 [synthetic] = read<u64, volatile, atomic=seq_cst>(%[[VALUE_b_120]]);
// DEFAULT-NEXT:                         write<u64, volatile, atomic=seq_cst>(%[[VALUE_a_120]], read<u64>(%[[VALUE250]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE250]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_120]]), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE251:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_121]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_121]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE252:[0-9]+]]: f32 [synthetic] = read<f32, volatile, atomic=seq_cst>(%[[VALUE_b_121]]);
// DEFAULT-NEXT:                         write<f32, volatile, atomic=seq_cst>(%[[VALUE_a_121]], read<f32>(%[[VALUE252]]));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE252]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_121]]), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE253:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_122]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_122]]), complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE254:[0-9]+]]: f64 [synthetic] = read<f64, volatile, atomic=seq_cst>(%[[VALUE_b_122]]);
// DEFAULT-NEXT:                         write<f64, volatile, atomic=seq_cst>(%[[VALUE_a_122]], read<f64>(%[[VALUE254]]));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE254]]), complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_122]]), complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE255:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_123]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_123]]), float_widen<f80, reason=explicit>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE256:[0-9]+]]: f80 [synthetic] = read<f80, volatile, atomic=seq_cst>(%[[VALUE_b_123]]);
// DEFAULT-NEXT:                         write<f80, volatile, atomic=seq_cst>(%[[VALUE_a_123]], read<f80>(%[[VALUE256]]));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80>(%[[VALUE256]]), float_widen<f80, reason=explicit>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_123]]), float_widen<f80, reason=explicit>(complex_to_real<f64, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE257:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_124]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_124]]), complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE258:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_b_124]]);
// DEFAULT-NEXT:                         write<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_124]], read<complex<f32>>(%[[VALUE258]]));
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE258]]), complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile, atomic=seq_cst>(%[[VALUE_a_124]]), complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=observable>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE259:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_125]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_125]]), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE260:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_b_125]]);
// DEFAULT-NEXT:                         write<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_125]], read<complex<f64>>(%[[VALUE260]]));
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE260]]), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f64>, exceptions=observable>(read<complex<f64>, volatile, atomic=seq_cst>(%[[VALUE_a_125]]), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE261:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_126]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_126]]), complex_convert<complex<f80>, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         let %[[VALUE262:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_b_126]]);
// DEFAULT-NEXT:                         write<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_126]], read<complex<f80>>(%[[VALUE262]]));
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE262]]), complex_convert<complex<f80>, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         if ne<complex<f80>, exceptions=observable>(read<complex<f80>, volatile, atomic=seq_cst>(%[[VALUE_a_126]]), complex_convert<complex<f80>, reason=explicit>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.5), index1 = const<f64>(3.5))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE263:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_127]]), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_b_127]]), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE264:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_b_127]]);
// DEFAULT-NEXT:                 write<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_127]], read<ptr<i32>>(%[[VALUE264]]));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE264]]), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_127]]), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE265:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_128]]), null<ptr<i32>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_b_128]]), addr_of<ptr<i32>>(%[[VALUE_i]]))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE266:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_b_128]]);
// DEFAULT-NEXT:                 write<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_128]], read<ptr<i32>>(%[[VALUE266]]));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE266]]), addr_of<ptr<i32>>(%[[VALUE_i]]))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_128]]), addr_of<ptr<i32>>(%[[VALUE_i]]))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_init:[0-9]+]] init: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_copy:[0-9]+]] copy: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s1:[0-9]+]] s1: atomic @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s2:[0-9]+]] s2: atomic @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE267:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE268:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE269:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE268]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE269]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(field0(%[[VALUE_init]])), read<i32>(%[[VALUE_j]]))), truncate<i16, reason=assign, fits=unknown>(read<i32>(%[[VALUE_j]])));
// DEFAULT-NEXT:         let %[[VALUE270:[0-9]+]]: @type[[TYPE_s]] [synthetic] = copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]]>(%[[VALUE_init]]));
// DEFAULT-NEXT:         write<@type[[TYPE_s]], atomic=seq_cst>(%[[VALUE_s1]], read<@type[[TYPE_s]]>(%[[VALUE270]]));
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_copy]], copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]]>(%[[VALUE270]])));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_init]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_copy]])), const<u64>(2048)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE271:[0-9]+]]: @type[[TYPE_s]] [synthetic] = copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]], atomic=seq_cst>(%[[VALUE_s1]]));
// DEFAULT-NEXT:         write<@type[[TYPE_s]], atomic=seq_cst>(%[[VALUE_s2]], read<@type[[TYPE_s]]>(%[[VALUE271]]));
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_copy]], copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]]>(%[[VALUE271]])));
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
