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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_2:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_3:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_4:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_5:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_6:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_7:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_8:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_9:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_10:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_11:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_12:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_13:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_14:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_15:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_16:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_17:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_18:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_19:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_20:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_21:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_22:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_23:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_24:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_25:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_26:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_27:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_28:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_29:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_30:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_31:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_32:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_33:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_34:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_35:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_36:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_37:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_38:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_39:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_40:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_41:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_42:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_43:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_44:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_45:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_46:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_47:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_48:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_49:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_50:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_51:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_52:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_53:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_54:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_55:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_56:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_57:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_58:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_59:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_60:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_61:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_62:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_63:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_64:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_65:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_66:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_67:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_68:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_69:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_70:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_71:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_72:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_73:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_74:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_75:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_76:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_77:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_78:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_79:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_80:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_81:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_82:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_83:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_84:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_85:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_86:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_87:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_88:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_89:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_90:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_91:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_92:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_93:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_94:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_95:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_96:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_97:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_98:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_99:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_100:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_101:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_102:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_103:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_104:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_105:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_106:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_107:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_108:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_109:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_110:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_111:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_112:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_113:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_114:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_115:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_116:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_117:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_118:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_119:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_120:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_121:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_122:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_123:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_124:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_125:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_126:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_127:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_128:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_129:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_130:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_131:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_132:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_133:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_134:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_135:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_136:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_137:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_138:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_139:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_140:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_141:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_142:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_143:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_144:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_145:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_146:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_147:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_148:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_149:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_150:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_151:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_152:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_153:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_154:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_155:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_156:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_157:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_158:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_159:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_160:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_161:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_162:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_163:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_164:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_165:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_166:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_167:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_168:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_169:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_170:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_171:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_172:[0-9]+]] a: volatile atomic i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_173:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_174:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_175:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_176:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_177:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_178:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_179:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_180:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_181:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_182:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_183:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_184:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_185:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_186:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_187:[0-9]+]] a: volatile atomic i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_188:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_189:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_190:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_191:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_192:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_193:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_194:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_195:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_196:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_197:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_198:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_199:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_200:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_201:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_202:[0-9]+]] a: volatile atomic i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_203:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_204:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_205:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_206:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_207:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_208:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_209:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_210:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_211:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_212:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_213:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_214:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_215:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_216:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_217:[0-9]+]] a: volatile atomic i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_218:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_219:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_220:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_221:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_222:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_223:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_224:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_225:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_226:[0-9]+]] a: volatile atomic bool [storage=static] = ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_227:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_228:[0-9]+]] a: volatile atomic i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_229:[0-9]+]] a: volatile atomic u8 [storage=static] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_230:[0-9]+]] a: volatile atomic i16 [storage=static] = truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_231:[0-9]+]] a: volatile atomic u16 [storage=static] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_232:[0-9]+]] a: volatile atomic i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_233:[0-9]+]] a: volatile atomic u32 [storage=static] = reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_234:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_235:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_236:[0-9]+]] a: volatile atomic i64 [storage=static] = widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_237:[0-9]+]] a: volatile atomic u64 [storage=static] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_238:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_239:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_240:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_241:[0-9]+]] a: volatile atomic bool [storage=static] = ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_242:[0-9]+]] a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_243:[0-9]+]] a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_244:[0-9]+]] a: volatile atomic u8 [storage=static] = truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_245:[0-9]+]] a: volatile atomic i16 [storage=static] = reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_246:[0-9]+]] a: volatile atomic u16 [storage=static] = truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_247:[0-9]+]] a: volatile atomic i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_248:[0-9]+]] a: volatile atomic u32 [storage=static] = truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_249:[0-9]+]] a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_250:[0-9]+]] a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_251:[0-9]+]] a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_252:[0-9]+]] a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_253:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_254:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_255:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_256:[0-9]+]] a: volatile atomic bool [storage=static] = ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_257:[0-9]+]] a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_258:[0-9]+]] a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_259:[0-9]+]] a: volatile atomic u8 [storage=static] = truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_260:[0-9]+]] a: volatile atomic i16 [storage=static] = reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_261:[0-9]+]] a: volatile atomic u16 [storage=static] = truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_262:[0-9]+]] a: volatile atomic i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_263:[0-9]+]] a: volatile atomic u32 [storage=static] = truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_264:[0-9]+]] a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_265:[0-9]+]] a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_266:[0-9]+]] a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_267:[0-9]+]] a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_268:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_269:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_270:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_271:[0-9]+]] a: volatile atomic bool [storage=static] = ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_272:[0-9]+]] a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_273:[0-9]+]] a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_274:[0-9]+]] a: volatile atomic u8 [storage=static] = truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_275:[0-9]+]] a: volatile atomic i16 [storage=static] = reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_276:[0-9]+]] a: volatile atomic u16 [storage=static] = truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_277:[0-9]+]] a: volatile atomic i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_278:[0-9]+]] a: volatile atomic u32 [storage=static] = truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_279:[0-9]+]] a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_280:[0-9]+]] a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_281:[0-9]+]] a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_282:[0-9]+]] a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_283:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_284:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_285:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_286:[0-9]+]] a: volatile atomic bool [storage=static] = ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_287:[0-9]+]] a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_288:[0-9]+]] a: volatile atomic i8 [storage=static] = reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_289:[0-9]+]] a: volatile atomic u8 [storage=static] = truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_290:[0-9]+]] a: volatile atomic i16 [storage=static] = reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_291:[0-9]+]] a: volatile atomic u16 [storage=static] = truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_292:[0-9]+]] a: volatile atomic i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_293:[0-9]+]] a: volatile atomic u32 [storage=static] = truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_294:[0-9]+]] a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_295:[0-9]+]] a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_296:[0-9]+]] a: volatile atomic i64 [storage=static] = reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_297:[0-9]+]] a: volatile atomic u64 [storage=static] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_298:[0-9]+]] a: volatile atomic f32 [storage=static] = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_299:[0-9]+]] a: volatile atomic f64 [storage=static] = int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_300:[0-9]+]] a: volatile atomic f80 [storage=static] = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_301:[0-9]+]] a: volatile atomic bool [storage=static] = ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_302:[0-9]+]] a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_303:[0-9]+]] a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_304:[0-9]+]] a: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_305:[0-9]+]] a: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_306:[0-9]+]] a: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_307:[0-9]+]] a: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_308:[0-9]+]] a: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_309:[0-9]+]] a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_310:[0-9]+]] a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_311:[0-9]+]] a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_312:[0-9]+]] a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_313:[0-9]+]] a: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_314:[0-9]+]] a: volatile atomic f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_315:[0-9]+]] a: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_316:[0-9]+]] a: volatile atomic bool [storage=static] = ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_317:[0-9]+]] a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_318:[0-9]+]] a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_319:[0-9]+]] a: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_320:[0-9]+]] a: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_321:[0-9]+]] a: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_322:[0-9]+]] a: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_323:[0-9]+]] a: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_324:[0-9]+]] a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_325:[0-9]+]] a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_326:[0-9]+]] a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_327:[0-9]+]] a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_328:[0-9]+]] a: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_329:[0-9]+]] a: volatile atomic f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_330:[0-9]+]] a: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_331:[0-9]+]] a: volatile atomic bool [storage=static] = ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_332:[0-9]+]] a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_333:[0-9]+]] a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_334:[0-9]+]] a: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_335:[0-9]+]] a: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_336:[0-9]+]] a: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_337:[0-9]+]] a: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_338:[0-9]+]] a: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_339:[0-9]+]] a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_340:[0-9]+]] a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_341:[0-9]+]] a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_342:[0-9]+]] a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_343:[0-9]+]] a: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_344:[0-9]+]] a: volatile atomic f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_345:[0-9]+]] a: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_346:[0-9]+]] a: volatile atomic bool [storage=static] = ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_347:[0-9]+]] a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_348:[0-9]+]] a: volatile atomic i8 [storage=static] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_349:[0-9]+]] a: volatile atomic u8 [storage=static] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_350:[0-9]+]] a: volatile atomic i16 [storage=static] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_351:[0-9]+]] a: volatile atomic u16 [storage=static] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_352:[0-9]+]] a: volatile atomic i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_353:[0-9]+]] a: volatile atomic u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_354:[0-9]+]] a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_355:[0-9]+]] a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_356:[0-9]+]] a: volatile atomic i64 [storage=static] = float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_357:[0-9]+]] a: volatile atomic u64 [storage=static] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_358:[0-9]+]] a: volatile atomic f32 [storage=static] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_359:[0-9]+]] a: volatile atomic f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_360:[0-9]+]] a: volatile atomic f80 [storage=static] = float_widen<f80, reason=explicit>(const<f64>(1.5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_ia:[0-9]+]] ia: array<i32, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_361:[0-9]+]] a: volatile atomic ptr<i32> [storage=static] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_362:[0-9]+]] a: volatile atomic ptr<i32> [storage=static] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_363:[0-9]+]] a: volatile atomic ptr<i32> [storage=static] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a_364:[0-9]+]] a: volatile atomic ptr<i32> [storage=static] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test_incdec:[0-9]+]] @test_incdec() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE4:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE4]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE6:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_2]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE6]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_2]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_3]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE8]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_3]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE10:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_4]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE10]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_4]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE12:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_5]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE12]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_5]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE14:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_6]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE14]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_6]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_7]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE16]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(0), const<i32>(1)), const<i32>(0)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_7]]), add<i32, overflow=ub>(const<i32>(0), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE18:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_8]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE18]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_8]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE20:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_9]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE20]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_9]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE22:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_10]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE22]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_10]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE24:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_11]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE24]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_11]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE26:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_12]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE26]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_12]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE28:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_13]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE28]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_13]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE30:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_14]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE30]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_14]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE32:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_15]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE32]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_15]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE35:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_16]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE35]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_16]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE37:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_17]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE37]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_17]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE38:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE39:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_18]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE39]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_18]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE40:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE41:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_19]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE41]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_19]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE43:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_20]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE43]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_20]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE44:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE45:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_21]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE45]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_21]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE46:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE47:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_22]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE47]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_22]]), add<i32, overflow=ub>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE48:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE49:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_23]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE49]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_23]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE50:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE51:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_24]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE51]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_24]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE53:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_25]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE53]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_25]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE54:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE55:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_26]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE55]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_26]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE56:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE57:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_27]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE57]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_27]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE58:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE59:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_28]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE59]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_28]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE60:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE61:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_29]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE61]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_29]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE62:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE63:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_30]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE63]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_30]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE64:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE65:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE66:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_31]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE66]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_31]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE67:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE68:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_32]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE68]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_32]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE69:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE70:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_33]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE70]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_33]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE71:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE72:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_34]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE72]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_34]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE73:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE74:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_35]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE74]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_35]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE75:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE76:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_36]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE76]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_36]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE77:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE78:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_37]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE78]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(0), const<i32>(1)), const<i32>(0)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_37]]), add<i32, overflow=ub>(const<i32>(0), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE79:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE80:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_38]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE80]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_38]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE81:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE82:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_39]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE82]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_39]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE83:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE84:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_40]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE84]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_40]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE85:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE86:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_41]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE86]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_41]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE87:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE88:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_42]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE88]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_42]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE89:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE90:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_43]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE90]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_43]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE91:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE92:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_44]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE92]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_44]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE93:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE94:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_45]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE94]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_45]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE95:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE96:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE97:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_46]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE97]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_46]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE98:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE99:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_47]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE99]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_47]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE100:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE101:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_48]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE101]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_48]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE102:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE103:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_49]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE103]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_49]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE104:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE105:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_50]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE105]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_50]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE106:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE107:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_51]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE107]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_51]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE108:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE109:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_52]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE109]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_52]]), add<i32, overflow=ub>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE110:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE111:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_53]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE111]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_53]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE112:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE113:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_54]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE113]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_54]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE114:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE115:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_55]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE115]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_55]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE116:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE117:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_56]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE117]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_56]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(0)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE118:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE119:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_57]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE119]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_57]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE120:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE121:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_58]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE121]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_58]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE122:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE123:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_59]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE123]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_59]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE124:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE125:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_60]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE125]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_60]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE126:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE127:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE128:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE129:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_61]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE129]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_61]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE130:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE131:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_62]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE131]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_62]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE132:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE133:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_63]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE133]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_63]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE134:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE135:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_64]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE135]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_64]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE136:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE137:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_65]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE137]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_65]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE138:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE139:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_66]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE139]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_66]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE140:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE141:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_67]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE141]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_67]]), add<i32, overflow=ub>(const<i32>(1), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE142:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE143:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_68]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE143]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_68]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE144:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE145:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_69]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE145]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_69]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE146:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE147:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_70]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE147]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_70]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE148:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE149:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_71]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE149]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_71]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE150:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE151:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_72]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE151]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_72]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE152:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE153:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_73]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE153]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_73]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE154:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE155:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_74]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE155]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_74]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE156:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE157:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_75]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE157]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_75]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE158:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE159:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE160:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_76]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE160]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_76]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE161:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE162:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_77]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE162]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_77]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE163:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE164:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_78]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE164]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_78]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE165:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE166:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_79]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE166]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_79]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE167:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE168:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_80]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE168]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_80]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE169:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE170:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_81]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE170]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_81]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE171:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE172:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_82]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE172]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_82]]), add<i32, overflow=ub>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE173:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE174:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_83]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE174]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_83]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE175:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE176:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_84]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE176]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_84]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE177:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE178:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_85]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE178]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_85]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE179:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE180:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_86]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE180]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_86]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE181:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE182:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_87]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE182]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_87]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE183:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE184:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_88]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE184]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_88]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE185:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE186:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_89]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE186]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_89]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE187:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE188:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_90]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE188]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_90]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE189:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE190:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE191:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_91]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE191]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_91]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE192:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE193:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_92]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE193]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_92]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE194:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE195:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_93]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE195]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_93]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE196:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE197:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_94]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE197]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_94]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE198:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE199:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_95]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE199]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_95]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE200:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE201:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_96]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE201]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_96]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE202:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE203:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_97]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE203]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_97]]), add<i32, overflow=ub>(const<i32>(1), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE204:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE205:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_98]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE205]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_98]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE206:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE207:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_99]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE207]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_99]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE208:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE209:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_100]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE209]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_100]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE210:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE211:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_101]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE211]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_101]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE212:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE213:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_102]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE213]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_102]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE214:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE215:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_103]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE215]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_103]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE216:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE217:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_104]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE217]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_104]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE218:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE219:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_105]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE219]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_105]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE220:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE221:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE222:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_106]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE222]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_106]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(1), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE223:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE224:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_107]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE224]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_107]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE225:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE226:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_108]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE226]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_108]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE227:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE228:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_109]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE228]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_109]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE229:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE230:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_110]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE230]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_110]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE231:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE232:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_111]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE232]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_111]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE233:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE234:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_112]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE234]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_112]]), add<i32, overflow=ub>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE235:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE236:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_113]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE236]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_113]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE237:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE238:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_114]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE238]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_114]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE239:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE240:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_115]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE240]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_115]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE241:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE242:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_116]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE242]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_116]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(1)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE243:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE244:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_117]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE244]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_117]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE245:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE246:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_118]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE246]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_118]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE247:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE248:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_119]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE248]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_119]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE249:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE250:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_120]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE250]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_120]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE251:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE252:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE253:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE254:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_121]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE254]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_121]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE255:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE256:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_122]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE256]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_122]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE257:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE258:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_123]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE258]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_123]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE259:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE260:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_124]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE260]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_124]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE261:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE262:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_125]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE262]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_125]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE263:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE264:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_126]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE264]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_126]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE265:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE266:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_127]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE266]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)), const<i32>(2)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_127]]), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE267:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE268:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_128]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE268]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_128]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE269:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE270:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_129]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE270]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_129]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE271:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE272:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_130]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE272]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_130]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE273:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE274:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_131]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE274]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_131]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE275:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE276:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_132]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE276]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_132]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE277:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE278:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_133]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE278]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_133]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE279:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE280:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_134]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE280]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_134]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE281:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE282:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_135]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE282]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_135]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE283:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE284:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE285:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_136]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE285]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_136]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE286:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE287:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_137]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE287]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_137]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE288:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE289:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_138]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE289]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_138]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE290:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE291:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_139]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE291]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_139]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE292:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE293:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_140]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE293]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_140]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE294:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE295:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_141]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE295]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_141]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE296:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE297:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_142]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE297]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(2), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(2)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_142]]), add<i32, overflow=ub>(const<i32>(2), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE298:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE299:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_143]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE299]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_143]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE300:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE301:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_144]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE301]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_144]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE302:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE303:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_145]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE303]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_145]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE304:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE305:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_146]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE305]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_146]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE306:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE307:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_147]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE307]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_147]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE308:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE309:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_148]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE309]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_148]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE310:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE311:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_149]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE311]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_149]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE312:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE313:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_150]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE313]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_150]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE314:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE315:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE316:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_151]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE316]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_151]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE317:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE318:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_152]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE318]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_152]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE319:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE320:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_153]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE320]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_153]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE321:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE322:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_154]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE322]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_154]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE323:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE324:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_155]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE324]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_155]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE325:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE326:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_156]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE326]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_156]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE327:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE328:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_157]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE328]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)), const<i32>(2)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_157]]), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE329:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE330:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_158]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE330]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_158]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE331:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE332:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_159]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE332]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_159]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE333:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE334:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_160]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE334]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_160]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE335:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE336:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_161]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE336]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_161]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE337:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE338:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_162]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE338]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_162]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE339:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE340:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_163]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE340]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_163]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE341:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE342:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_164]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE342]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_164]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE343:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE344:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_165]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE344]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_165]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE345:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE346:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE347:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_166]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE347]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_166]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE348:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE349:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_167]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE349]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_167]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE350:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE351:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_168]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE351]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_168]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE352:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE353:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_169]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE353]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_169]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE354:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE355:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_170]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE355]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_170]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE356:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE357:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_171]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE357]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_171]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE358:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE359:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_172]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE359]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(const<i32>(2), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(2)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_172]]), add<i32, overflow=ub>(const<i32>(2), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE360:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE361:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_173]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE361]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_173]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE362:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE363:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_174]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE363]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_174]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE364:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE365:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_175]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE365]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_175]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE366:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE367:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_176]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE367]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_176]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(const<i32>(2)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE368:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE369:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_177]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE369]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_177]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE370:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE371:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_178]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE371]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_178]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE372:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE373:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_179]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE373]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_179]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE374:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE375:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_180]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE375]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_180]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE376:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE377:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE378:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE379:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_181]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE379]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_181]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE380:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE381:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_182]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE381]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_182]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE382:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE383:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_183]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE383]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_183]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE384:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE385:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_184]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE385]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_184]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE386:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE387:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_185]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE387]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_185]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE388:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE389:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_186]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE389]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_186]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE390:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE391:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_187]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE391]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_187]]), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE392:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE393:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_188]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE393]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_188]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE394:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE395:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_189]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE395]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_189]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE396:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE397:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_190]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE397]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_190]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE398:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE399:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_191]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE399]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_191]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE400:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE401:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_192]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE401]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_192]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE402:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE403:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_193]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE403]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_193]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE404:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE405:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_194]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE405]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_194]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE406:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE407:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_195]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE407]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_195]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE408:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE409:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE410:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_196]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE410]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_196]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE411:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE412:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_197]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE412]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_197]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE413:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE414:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_198]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE414]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_198]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE415:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE416:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_199]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE416]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_199]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE417:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE418:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_200]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE418]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_200]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE419:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE420:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_201]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE420]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_201]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE421:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE422:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_202]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE422]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_202]]), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE423:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE424:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_203]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE424]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_203]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE425:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE426:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_204]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE426]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_204]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE427:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE428:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_205]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE428]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_205]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE429:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE430:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_206]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE430]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_206]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE431:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE432:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_207]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE432]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_207]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE433:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE434:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_208]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE434]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_208]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE435:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE436:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_209]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE436]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_209]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE437:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE438:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_210]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE438]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_210]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE439:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE440:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE441:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_211]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE441]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_211]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE442:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE443:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_212]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE443]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_212]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE444:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE445:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_213]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE445]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_213]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE446:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE447:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_214]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE447]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_214]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE448:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE449:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_215]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE449]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_215]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE450:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE451:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_216]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE451]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_216]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE452:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE453:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_217]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE453]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_217]]), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE454:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE455:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_218]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE455]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_218]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE456:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE457:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_219]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE457]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_219]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE458:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE459:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_220]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE459]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_220]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE460:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE461:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_221]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE461]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_221]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE462:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE463:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_222]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE463]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_222]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE464:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE465:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_223]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE465]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_223]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE466:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE467:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_224]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE467]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_224]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE468:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE469:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_225]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE469]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_225]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE470:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE471:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE472:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_226]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE472]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_226]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE473:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE474:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_227]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE474]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_227]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE475:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE476:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_228]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE476]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_228]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE477:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE478:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_229]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE478]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_229]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE479:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE480:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_230]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE480]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_230]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE481:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE482:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_231]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE482]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_231]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE483:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE484:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_232]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE484]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_232]]), add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE485:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE486:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_233]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE486]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_233]]), add<u32, overflow=wrap>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE487:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE488:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_234]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE488]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_234]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE489:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE490:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_235]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE490]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_235]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE491:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE492:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_236]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE492]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_236]]), add<i64, overflow=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE493:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE494:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_237]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE494]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_237]]), add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE495:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE496:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_238]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE496]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_238]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE497:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE498:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_239]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE498]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_239]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE499:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE500:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_240]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE500]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_240]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE501:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE502:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE503:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE504:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_241]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE504]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_241]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE505:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE506:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_242]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE506]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_242]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE507:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE508:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_243]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE508]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_243]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE509:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE510:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_244]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE510]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_244]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE511:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE512:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_245]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE512]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_245]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE513:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE514:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_246]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE514]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_246]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE515:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE516:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_247]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE516]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), const<i32>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_247]]), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE517:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE518:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_248]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE518]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_248]]), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE519:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE520:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_249]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE520]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_249]]), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE521:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE522:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_250]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE522]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_250]]), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE523:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE524:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_251]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE524]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_251]]), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE525:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE526:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_252]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE526]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_252]]), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE527:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE528:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_253]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE528]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_253]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE529:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE530:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_254]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE530]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_254]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE531:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE532:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_255]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE532]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_255]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE533:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE534:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE535:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_256]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE535]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_256]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE536:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE537:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_257]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE537]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_257]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE538:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE539:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_258]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE539]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_258]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE540:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE541:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_259]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE541]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_259]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE542:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE543:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_260]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE543]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_260]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE544:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE545:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_261]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE545]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_261]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE546:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE547:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_262]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE547]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), neg<i32, overflow=ub>(const<i32>(1))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_262]]), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE548:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE549:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_263]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE549]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_263]]), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE550:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE551:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_264]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE551]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_264]]), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE552:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE553:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_265]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE553]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_265]]), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE554:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE555:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_266]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE555]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_266]]), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE556:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE557:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_267]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE557]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_267]]), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE558:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE559:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_268]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE559]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_268]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE560:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE561:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_269]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE561]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_269]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE562:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE563:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_270]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE563]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_270]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE564:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE565:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE566:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_271]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE566]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_271]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE567:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE568:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_272]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE568]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_272]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE569:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE570:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_273]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE570]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_273]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE571:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE572:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_274]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE572]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_274]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE573:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE574:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_275]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE574]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_275]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE575:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE576:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_276]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE576]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_276]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE577:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE578:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_277]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE578]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), const<i32>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_277]]), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE579:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE580:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_278]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE580]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_278]]), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE581:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE582:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_279]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE582]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_279]]), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE583:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE584:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_280]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE584]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_280]]), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE585:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE586:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_281]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE586]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_281]]), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE587:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE588:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_282]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE588]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_282]]), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE589:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE590:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_283]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE590]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_283]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE591:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE592:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_284]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE592]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_284]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE593:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE594:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_285]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE594]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_285]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE595:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE596:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE597:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_286]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE597]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_286]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<u64, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), const<u64>(0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE598:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE599:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_287]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE599]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_287]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE600:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE601:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_288]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE601]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_288]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE602:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE603:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_289]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE603]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_289]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE604:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE605:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_290]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE605]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_290]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE606:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE607:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_291]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE607]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_291]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE608:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE609:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_292]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE609]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), neg<i32, overflow=ub>(const<i32>(1))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_292]]), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE610:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE611:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_293]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE611]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_293]]), add<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE612:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE613:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_294]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE613]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_294]]), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE614:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE615:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_295]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE615]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_295]]), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE616:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE617:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_296]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE617]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_296]]), add<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE618:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE619:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_297]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE619]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_297]]), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE620:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE621:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_298]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE621]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_298]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE622:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE623:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_299]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE623]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_299]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE624:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE625:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_300]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE625]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_300]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(60))), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE626:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE627:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE628:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE629:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_301]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE629]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_301]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE630:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE631:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_302]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE631]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_302]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE632:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE633:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_303]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE633]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_303]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE634:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE635:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_304]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE635]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_304]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE636:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE637:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_305]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE637]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_305]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE638:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE639:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_306]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE639]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_306]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE640:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE641:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_307]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE641]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), const<i32>(1)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_307]]), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE642:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE643:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_308]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE643]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_308]]), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE644:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE645:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_309]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE645]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_309]]), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE646:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE647:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_310]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE647]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_310]]), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE648:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE649:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_311]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE649]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_311]]), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE650:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE651:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_312]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE651]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_312]]), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE652:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE653:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_313]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE653]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_313]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE654:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE655:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_314]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE655]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<f64>(1.5)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_314]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE656:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE657:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_315]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE657]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_315]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE658:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE659:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE660:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_316]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE660]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_316]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE661:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE662:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_317]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE662]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_317]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE663:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE664:[0-9]+]]: i8 [synthetic] = update<i8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_318]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE664]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_318]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE665:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE666:[0-9]+]]: u8 [synthetic] = update<u8, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_319]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE666]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_319]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE667:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE668:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_320]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE668]])), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_320]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE669:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE670:[0-9]+]]: u16 [synthetic] = update<u16, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_321]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE670]]))), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_321]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE671:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE672:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_322]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE672]]), conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), neg<i32, overflow=ub>(const<i32>(1))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_322]]), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE673:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE674:[0-9]+]]: u32 [synthetic] = update<u32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_323]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE674]]), conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_323]]), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE675:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE676:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_324]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE676]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_324]]), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE677:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE678:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_325]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE678]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_325]]), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE679:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE680:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_326]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE680]]), conditional<i64>(ne<i32>(const<i32>(1), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_326]]), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE681:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE682:[0-9]+]]: u64 [synthetic] = update<u64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_327]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE682]]), conditional<u64>(ne<i32>(const<i32>(1), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_327]]), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE683:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE684:[0-9]+]]: f32 [synthetic] = update<f32, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_328]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE684]]), conditional<f32>(ne<i32>(const<i32>(1), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_328]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE685:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE686:[0-9]+]]: f64 [synthetic] = update<f64, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_329]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE686]]), conditional<f64>(ne<i32>(const<i32>(1), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), const<f64>(1.5)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_329]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE687:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE688:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_330]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE688]]), conditional<f80>(ne<i32>(const<i32>(1), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_330]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE689:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE690:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE691:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_331]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE691]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0))), const<i32>(1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_331]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0))), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE692:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE693:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_332]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE693]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_332]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE694:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE695:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_333]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE695]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_333]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE696:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE697:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_334]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE697]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_334]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE698:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE699:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_335]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE699]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_335]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE700:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE701:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_336]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE701]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_336]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), const<i32>(1)))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE702:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE703:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_337]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE703]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), const<i32>(1)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_337]]), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), const<i32>(1)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE704:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE705:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_338]], add<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE705]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_338]]), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE706:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE707:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_339]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE707]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_339]]), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE708:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE709:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_340]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE709]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_340]]), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE710:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE711:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_341]], add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE711]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_341]]), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE712:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE713:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_342]], add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE713]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_342]]), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE714:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE715:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_343]], add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE715]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_343]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE716:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE717:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_344]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE717]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<f64>(1.5)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_344]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE718:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE719:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_345]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE719]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_345]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE720:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE721:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE722:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_346]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE722]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_a_346]])), from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(1.5), const<f64>(0.0))), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE723:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE724:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_347]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE724]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_347]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE725:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE726:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_348]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i8>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE726]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i8, volatile, atomic=seq_cst>(%[[VALUE_a_348]])), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE727:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE728:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_349]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u8>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE728]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile, atomic=seq_cst>(%[[VALUE_a_349]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE729:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE730:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_350]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE730]])), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(widen<i32, reason=promotion>(read<i16, volatile, atomic=seq_cst>(%[[VALUE_a_350]])), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))), neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE731:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE732:[0-9]+]]: u16 [synthetic] = update<u16, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_351]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(old<u16>)), const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE732]]))), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile, atomic=seq_cst>(%[[VALUE_a_351]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)))), neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE733:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE734:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_352]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE734]]), conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), neg<i32, overflow=ub>(const<i32>(1))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_a_352]]), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE735:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE736:[0-9]+]]: u32 [synthetic] = update<u32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_353]], sub<u32, overflow=wrap>(old<u32>, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE736]]), conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u32>(read<u32, volatile, atomic=seq_cst>(%[[VALUE_a_353]]), add<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE737:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE738:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_354]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE738]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_354]]), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE739:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE740:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_355]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE740]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_355]]), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE741:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE742:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_356]], sub<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE742]]), conditional<i64>(ne<i32>(const<i32>(0), const<i32>(0)), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i64>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_a_356]]), add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE743:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE744:[0-9]+]]: u64 [synthetic] = update<u64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_357]], sub<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 if ne<u64>(read<u64>(%[[VALUE744]]), conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<u64>(read<u64, volatile, atomic=seq_cst>(%[[VALUE_a_357]]), add<u64, overflow=wrap>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE745:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE746:[0-9]+]]: f32 [synthetic] = update<f32, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_358]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE746]]), conditional<f32>(ne<i32>(const<i32>(0), const<i32>(0)), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f32, exceptions=ignore>(read<f32, volatile, atomic=seq_cst>(%[[VALUE_a_358]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE747:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE748:[0-9]+]]: f64 [synthetic] = update<f64, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_359]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE748]]), conditional<f64>(ne<i32>(const<i32>(0), const<i32>(0)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), const<f64>(1.5)))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f64, exceptions=ignore>(read<f64, volatile, atomic=seq_cst>(%[[VALUE_a_359]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         do %[[VALUE749:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE750:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_360]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE750]]), conditional<f80>(ne<i32>(const<i32>(0), const<i32>(0)), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), float_widen<f80, reason=explicit>(const<f64>(1.5))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<f80, exceptions=ignore>(read<f80, volatile, atomic=seq_cst>(%[[VALUE_a_360]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f80, reason=explicit>(const<f64>(1.5)), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE751:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE752:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_361]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE752]]), conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))), const<i32>(1)), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_361]]), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE753:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE754:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=new, volatile, atomic=seq_cst>(%[[VALUE_a_362]], ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE754]]), conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_362]]), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE755:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE756:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_363]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE756]]), conditional<ptr<i32>>(ne<i32>(const<i32>(0), const<i32>(0)), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))), const<i32>(1)), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_363]]), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE757:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE758:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=old, volatile, atomic=seq_cst>(%[[VALUE_a_364]], ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE758]]), conditional<ptr<i32>>(ne<i32>(const<i32>(0), const<i32>(0)), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<ptr<i32>>(read<ptr<i32>, volatile, atomic=seq_cst>(%[[VALUE_a_364]]), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_ia]]), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_incdec]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
