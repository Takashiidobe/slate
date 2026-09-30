/* Verify that -Walloc-size-greater-than doesn't cause false positives
   for anti-ranges.  Note that not all of the statements used to create
   anti-ranges below result in the argument being represented as an anti
   range.

   { dg-do compile }
   { dg-options "-O2 -Walloc-size-larger-than=12" } 
   { dg-options "-Wno-overflow" { target { ! int32plus } } } */

#define SCHAR_MAX __SCHAR_MAX__
#define SCHAR_MIN (-SCHAR_MAX - 1)
#define UCHAR_MAX (SCHAR_MAX * 2 + 1)

#define SHRT_MAX  __SHRT_MAX__
#define SHRT_MIN  (-SHRT_MAX - 1)
#define USHRT_MAX (SHRT_MAX * 2U + 1)

#define INT_MAX   __INT_MAX__
#define INT_MIN   (-INT_MAX - 1)
#define UINT_MAX  (INT_MAX * 2U + 1)

#define LONG_MAX __LONG_MAX__
#define LONG_MIN (-LONG_MAX - 1)
#define ULONG_MAX (LONG_MAX * 2LU + 1)

#define PTRDIFF_MAX __PTRDIFF_MAX__
#define PTRDIFF_MIN (-PTRDIFF_MAX - 1)
#define SIZE_MAX    __SIZE_MAX__

typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__    size_t;

#define CONCAT(a, b)  a ## b
#define CAT(a, b)     CONCAT (a, b)

/* Macro to generate a unique function to test the anti-range
   ~[MIN, MAX] for type T.  */
#define TEST(T, min, max)					\
  void* CAT (test_anti_range_, __LINE__)(T n)			\
  {								\
    extern void* CAT (alloc_anti_range_, __LINE__)(T)		\
      __attribute__ ((alloc_size (1)));				\
    if (min <= n && n <= max)					\
      n = min - 1;						\
    return CAT (alloc_anti_range_, __LINE__)(n);		\
  } typedef void dummy   /* Require a semicolon.  */


/* Verify the anti-range ~[TYPE_MAX - 1, TYPE_MAX - 1].  */
TEST (signed char, SCHAR_MAX - 1, SCHAR_MAX - 1);
TEST (unsigned char, UCHAR_MAX - 1, UCHAR_MAX - 1);
TEST (short, SHRT_MAX - 1, SHRT_MAX - 1);
TEST (unsigned short, USHRT_MAX - 1, USHRT_MAX - 1);
TEST (int, INT_MAX - 1, INT_MAX - 1);
TEST (unsigned, UINT_MAX - 1, UINT_MAX - 1);
TEST (long, LONG_MAX - 1, LONG_MAX - 1);
TEST (unsigned long, ULONG_MAX - 1, ULONG_MAX - 1);
TEST (ptrdiff_t, PTRDIFF_MAX - 1, PTRDIFF_MAX - 1);
TEST (size_t, SIZE_MAX - 1, SIZE_MAX - 1);

/* Verify ~[0, 0].  */
TEST (signed char, 0, 0);
TEST (unsigned char, 0, 0);
TEST (short, 0, 0);
TEST (unsigned short, 0, 0);
TEST (int, 0, 0);
TEST (unsigned, 0, 0);
TEST (long, 0, 0);
TEST (unsigned long, 0, 0);
TEST (ptrdiff_t, 0, 0);
TEST (size_t, 0, 0);

/* Verify ~[1, 1].  */
TEST (signed char, 1, 1);
TEST (unsigned char, 1, 1);
TEST (short, 1, 1);
TEST (unsigned short, 1, 1);
TEST (int, 1, 1);
TEST (unsigned, 1, 1);
TEST (long, 1, 1);
TEST (unsigned long, 1, 1);
TEST (ptrdiff_t, 1, 1);
TEST (size_t, 1, 1);


/* Verify ~[TYPE_MAX - 2, TYPE_MAX - 1].  */
TEST (signed char, SCHAR_MAX - 2, SCHAR_MAX - 1);
TEST (unsigned char, UCHAR_MAX - 2, UCHAR_MAX - 1);
TEST (short, SHRT_MAX - 2, SHRT_MAX - 1);
TEST (unsigned short, USHRT_MAX - 2, USHRT_MAX - 1);
TEST (int, INT_MAX - 2, INT_MAX - 1);
TEST (unsigned, UINT_MAX - 2, UINT_MAX - 1);
TEST (long, LONG_MAX - 2, LONG_MAX - 1);
TEST (unsigned long, ULONG_MAX - 2, ULONG_MAX - 1);
TEST (ptrdiff_t, PTRDIFF_MAX - 2, PTRDIFF_MAX - 1);
TEST (size_t, SIZE_MAX - 2, SIZE_MAX - 1);

/* Verify ~[0, 2].  */
TEST (signed char, 0, 2);
TEST (unsigned char, 0, 2);
TEST (short, 0, 2);
TEST (unsigned short, 0, 2);
TEST (int, 0, 2);
TEST (unsigned int, 0, 2);
TEST (long, 0, 2);
TEST (unsigned long, 0, 2);
TEST (ptrdiff_t, 0, 2);
TEST (size_t, 0, 2);

/* Verify the signed anti-range ~[TYPE_MIN - 2, -1].  */
TEST (signed char, SCHAR_MIN + 2, -1);
TEST (short, SHRT_MIN + 2, -1);
TEST (int, INT_MIN + 2, -1);
TEST (long, LONG_MIN + 2, -1);
TEST (ptrdiff_t, PTRDIFF_MIN + 2, -1);

/* Verify the signed anti-range ~[TYPE_MIN - 2, 0].  */
TEST (signed char, SCHAR_MIN + 2, 0);
TEST (short, SHRT_MIN + 2, 0);
TEST (int, INT_MIN + 2, 0);
TEST (long, LONG_MIN + 2, 0);
TEST (ptrdiff_t, PTRDIFF_MIN + 2, 0);

/* Verify the signed anti-range ~[TYPE_MIN - 2, 1].  */
TEST (signed char, SCHAR_MIN + 2, 1);
TEST (short, SHRT_MIN + 2, 1);
TEST (int, INT_MIN + 2, 1);
TEST (long, LONG_MIN + 2, 1);
TEST (ptrdiff_t, PTRDIFF_MIN + 2, 1);

/* Verify the signed anti-range ~[TYPE_MIN - 2, 2].  */
TEST (signed char, SCHAR_MIN + 2, 2);
TEST (short, SHRT_MIN + 2, 2);
TEST (int, INT_MIN + 2, 2);
TEST (long, LONG_MIN + 2, 2);
TEST (ptrdiff_t, PTRDIFF_MIN + 2, 2);

/* Verify the signed anti-range ~[-1, 2].  */
TEST (signed char, -1, 2);
TEST (short, -1, 2);
TEST (int, -1, 2);
TEST (long, -1, 2);
TEST (ptrdiff_t, 01, 2);

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_ptrdiff_t:[0-9]+]] ptrdiff_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_dummy:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_2:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_3:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_4:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_5:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_6:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_7:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_8:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_9:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_10:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_11:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_12:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_13:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_14:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_15:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_16:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_17:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_18:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_19:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_20:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_21:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_22:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_23:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_24:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_25:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_26:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_27:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_28:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_29:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_30:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_31:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_32:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_33:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_34:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_35:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_36:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_37:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_38:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_39:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_40:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_41:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_42:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_43:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_44:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_45:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_46:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_47:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_48:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_49:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_50:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_51:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_52:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_53:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_54:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_55:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_56:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_57:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_58:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_59:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_60:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_61:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_62:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_63:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_64:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_65:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_66:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_67:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_68:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_69:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_70:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_71:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_72:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_73:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_74:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     type @type[[TYPE_dummy_75:[0-9]+]] dummy = void;
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_50:[0-9]+]] @alloc_anti_range_50(%[[VALUE0:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_50:[0-9]+]] @test_anti_range_50(%[[VALUE_n:[0-9]+]] n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n]])), sub<i32, overflow=ub>(const<i32>(127), const<i32>(1))))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_alloc_anti_range_50]], read<i8>(%[[VALUE_n]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_51:[0-9]+]] @alloc_anti_range_51(%[[VALUE1:[0-9]+]] <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_51:[0-9]+]] @test_anti_range_51(%[[VALUE_n_2:[0-9]+]] n: u8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_2]])))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_2]]))), sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1))))
// DEFAULT-NEXT:             write<u8>(%[[VALUE_n_2]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1)), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_alloc_anti_range_51]], read<u8>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_52:[0-9]+]] @alloc_anti_range_52(%[[VALUE2:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_52:[0-9]+]] @test_anti_range_52(%[[VALUE_n_3:[0-9]+]] n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_3]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_3]])), sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1))))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_3]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_alloc_anti_range_52]], read<i16>(%[[VALUE_n_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_53:[0-9]+]] @alloc_anti_range_53(%[[VALUE3:[0-9]+]] <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_53:[0-9]+]] @test_anti_range_53(%[[VALUE_n_4:[0-9]+]] n: u16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_4]]))))), le<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_4]])))), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u16>(%[[VALUE_n_4]], truncate<u16, reason=assign, fits=unknown>(sub<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_alloc_anti_range_53]], read<u16>(%[[VALUE_n_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_54:[0-9]+]] @alloc_anti_range_54(%[[VALUE4:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_54:[0-9]+]] @test_anti_range_54(%[[VALUE_n_5:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1)), read<i32>(%[[VALUE_n_5]])), le<i32>(read<i32>(%[[VALUE_n_5]]), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1))))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_5]], sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_54]], read<i32>(%[[VALUE_n_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_55:[0-9]+]] @alloc_anti_range_55(%[[VALUE5:[0-9]+]] <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_55:[0-9]+]] @test_anti_range_55(%[[VALUE_n_6:[0-9]+]] n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), read<u32>(%[[VALUE_n_6]])), le<u32>(read<u32>(%[[VALUE_n_6]]), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_n_6]], sub<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_alloc_anti_range_55]], read<u32>(%[[VALUE_n_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_56:[0-9]+]] @alloc_anti_range_56(%[[VALUE6:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_56:[0-9]+]] @test_anti_range_56(%[[VALUE_n_7:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))), read<i64>(%[[VALUE_n_7]])), le<i64>(read<i64>(%[[VALUE_n_7]]), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_7]], sub<i64, overflow=ub>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_56]], read<i64>(%[[VALUE_n_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_57:[0-9]+]] @alloc_anti_range_57(%[[VALUE7:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_57:[0-9]+]] @test_anti_range_57(%[[VALUE_n_8:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_n_8]])), le<u64>(read<u64>(%[[VALUE_n_8]]), sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_8]], sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_57]], read<u64>(%[[VALUE_n_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_58:[0-9]+]] @alloc_anti_range_58(%[[VALUE8:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_58:[0-9]+]] @test_anti_range_58(%[[VALUE_n_9:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))), read<i64>(%[[VALUE_n_9]])), le<i64>(read<i64>(%[[VALUE_n_9]]), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_9]], sub<i64, overflow=ub>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_58]], read<i64>(%[[VALUE_n_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_59:[0-9]+]] @alloc_anti_range_59(%[[VALUE9:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_59:[0-9]+]] @test_anti_range_59(%[[VALUE_n_10:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_n_10]])), le<u64>(read<u64>(%[[VALUE_n_10]]), sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_10]], sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_59]], read<u64>(%[[VALUE_n_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_62:[0-9]+]] @alloc_anti_range_62(%[[VALUE10:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_62:[0-9]+]] @test_anti_range_62(%[[VALUE_n_11:[0-9]+]] n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_11]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_11]])), const<i32>(0)))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n_11]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_alloc_anti_range_62]], read<i8>(%[[VALUE_n_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_63:[0-9]+]] @alloc_anti_range_63(%[[VALUE11:[0-9]+]] <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_63:[0-9]+]] @test_anti_range_63(%[[VALUE_n_12:[0-9]+]] n: u8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_12]])))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_12]]))), const<i32>(0)))
// DEFAULT-NEXT:             write<u8>(%[[VALUE_n_12]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_alloc_anti_range_63]], read<u8>(%[[VALUE_n_12]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_64:[0-9]+]] @alloc_anti_range_64(%[[VALUE12:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_64:[0-9]+]] @test_anti_range_64(%[[VALUE_n_13:[0-9]+]] n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_13]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_13]])), const<i32>(0)))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_13]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_alloc_anti_range_64]], read<i16>(%[[VALUE_n_13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_65:[0-9]+]] @alloc_anti_range_65(%[[VALUE13:[0-9]+]] <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_65:[0-9]+]] @test_anti_range_65(%[[VALUE_n_14:[0-9]+]] n: u16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_14]])))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_14]]))), const<i32>(0)))
// DEFAULT-NEXT:             write<u16>(%[[VALUE_n_14]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_alloc_anti_range_65]], read<u16>(%[[VALUE_n_14]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_66:[0-9]+]] @alloc_anti_range_66(%[[VALUE14:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_66:[0-9]+]] @test_anti_range_66(%[[VALUE_n_15:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), read<i32>(%[[VALUE_n_15]])), le<i32>(read<i32>(%[[VALUE_n_15]]), const<i32>(0)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_15]], sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_66]], read<i32>(%[[VALUE_n_15]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_67:[0-9]+]] @alloc_anti_range_67(%[[VALUE15:[0-9]+]] <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_67:[0-9]+]] @test_anti_range_67(%[[VALUE_n_16:[0-9]+]] n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)), read<u32>(%[[VALUE_n_16]])), le<u32>(read<u32>(%[[VALUE_n_16]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_n_16]], reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_alloc_anti_range_67]], read<u32>(%[[VALUE_n_16]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_68:[0-9]+]] @alloc_anti_range_68(%[[VALUE16:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_68:[0-9]+]] @test_anti_range_68(%[[VALUE_n_17:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(0)), read<i64>(%[[VALUE_n_17]])), le<i64>(read<i64>(%[[VALUE_n_17]]), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_17]], widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_68]], read<i64>(%[[VALUE_n_17]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_69:[0-9]+]] @alloc_anti_range_69(%[[VALUE17:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_69:[0-9]+]] @test_anti_range_69(%[[VALUE_n_18:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%[[VALUE_n_18]])), le<u64>(read<u64>(%[[VALUE_n_18]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_18]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_69]], read<u64>(%[[VALUE_n_18]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_70:[0-9]+]] @alloc_anti_range_70(%[[VALUE18:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_70:[0-9]+]] @test_anti_range_70(%[[VALUE_n_19:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(0)), read<i64>(%[[VALUE_n_19]])), le<i64>(read<i64>(%[[VALUE_n_19]]), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_19]], widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_70]], read<i64>(%[[VALUE_n_19]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_71:[0-9]+]] @alloc_anti_range_71(%[[VALUE19:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_71:[0-9]+]] @test_anti_range_71(%[[VALUE_n_20:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%[[VALUE_n_20]])), le<u64>(read<u64>(%[[VALUE_n_20]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_20]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_71]], read<u64>(%[[VALUE_n_20]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_74:[0-9]+]] @alloc_anti_range_74(%[[VALUE20:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_74:[0-9]+]] @test_anti_range_74(%[[VALUE_n_21:[0-9]+]] n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_21]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_21]])), const<i32>(1)))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n_21]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_alloc_anti_range_74]], read<i8>(%[[VALUE_n_21]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_75:[0-9]+]] @alloc_anti_range_75(%[[VALUE21:[0-9]+]] <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_75:[0-9]+]] @test_anti_range_75(%[[VALUE_n_22:[0-9]+]] n: u8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_22]])))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_22]]))), const<i32>(1)))
// DEFAULT-NEXT:             write<u8>(%[[VALUE_n_22]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_alloc_anti_range_75]], read<u8>(%[[VALUE_n_22]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_76:[0-9]+]] @alloc_anti_range_76(%[[VALUE22:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_76:[0-9]+]] @test_anti_range_76(%[[VALUE_n_23:[0-9]+]] n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_23]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_23]])), const<i32>(1)))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_23]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_alloc_anti_range_76]], read<i16>(%[[VALUE_n_23]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_77:[0-9]+]] @alloc_anti_range_77(%[[VALUE23:[0-9]+]] <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_77:[0-9]+]] @test_anti_range_77(%[[VALUE_n_24:[0-9]+]] n: u16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_24]])))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_24]]))), const<i32>(1)))
// DEFAULT-NEXT:             write<u16>(%[[VALUE_n_24]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_alloc_anti_range_77]], read<u16>(%[[VALUE_n_24]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_78:[0-9]+]] @alloc_anti_range_78(%[[VALUE24:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_78:[0-9]+]] @test_anti_range_78(%[[VALUE_n_25:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), read<i32>(%[[VALUE_n_25]])), le<i32>(read<i32>(%[[VALUE_n_25]]), const<i32>(1)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_25]], sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_78]], read<i32>(%[[VALUE_n_25]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_79:[0-9]+]] @alloc_anti_range_79(%[[VALUE25:[0-9]+]] <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_79:[0-9]+]] @test_anti_range_79(%[[VALUE_n_26:[0-9]+]] n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), read<u32>(%[[VALUE_n_26]])), le<u32>(read<u32>(%[[VALUE_n_26]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_n_26]], reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_alloc_anti_range_79]], read<u32>(%[[VALUE_n_26]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_80:[0-9]+]] @alloc_anti_range_80(%[[VALUE26:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_80:[0-9]+]] @test_anti_range_80(%[[VALUE_n_27:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(1)), read<i64>(%[[VALUE_n_27]])), le<i64>(read<i64>(%[[VALUE_n_27]]), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_27]], widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_80]], read<i64>(%[[VALUE_n_27]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_81:[0-9]+]] @alloc_anti_range_81(%[[VALUE27:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_81:[0-9]+]] @test_anti_range_81(%[[VALUE_n_28:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), read<u64>(%[[VALUE_n_28]])), le<u64>(read<u64>(%[[VALUE_n_28]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_28]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_81]], read<u64>(%[[VALUE_n_28]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_82:[0-9]+]] @alloc_anti_range_82(%[[VALUE28:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_82:[0-9]+]] @test_anti_range_82(%[[VALUE_n_29:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(1)), read<i64>(%[[VALUE_n_29]])), le<i64>(read<i64>(%[[VALUE_n_29]]), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_29]], widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_82]], read<i64>(%[[VALUE_n_29]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_83:[0-9]+]] @alloc_anti_range_83(%[[VALUE29:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_83:[0-9]+]] @test_anti_range_83(%[[VALUE_n_30:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), read<u64>(%[[VALUE_n_30]])), le<u64>(read<u64>(%[[VALUE_n_30]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_30]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_83]], read<u64>(%[[VALUE_n_30]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_87:[0-9]+]] @alloc_anti_range_87(%[[VALUE30:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_87:[0-9]+]] @test_anti_range_87(%[[VALUE_n_31:[0-9]+]] n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_31]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_31]])), sub<i32, overflow=ub>(const<i32>(127), const<i32>(1))))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n_31]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_alloc_anti_range_87]], read<i8>(%[[VALUE_n_31]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_88:[0-9]+]] @alloc_anti_range_88(%[[VALUE31:[0-9]+]] <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_88:[0-9]+]] @test_anti_range_88(%[[VALUE_n_32:[0-9]+]] n: u8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(2)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_32]])))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_32]]))), sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1))))
// DEFAULT-NEXT:             write<u8>(%[[VALUE_n_32]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(2)), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_alloc_anti_range_88]], read<u8>(%[[VALUE_n_32]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_89:[0-9]+]] @alloc_anti_range_89(%[[VALUE32:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_89:[0-9]+]] @test_anti_range_89(%[[VALUE_n_33:[0-9]+]] n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_33]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_33]])), sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1))))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_33]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_alloc_anti_range_89]], read<i16>(%[[VALUE_n_33]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_90:[0-9]+]] @alloc_anti_range_90(%[[VALUE33:[0-9]+]] <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_90:[0-9]+]] @test_anti_range_90(%[[VALUE_n_34:[0-9]+]] n: u16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_34]]))))), le<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_34]])))), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u16>(%[[VALUE_n_34]], truncate<u16, reason=assign, fits=unknown>(sub<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_alloc_anti_range_90]], read<u16>(%[[VALUE_n_34]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_91:[0-9]+]] @alloc_anti_range_91(%[[VALUE34:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_91:[0-9]+]] @test_anti_range_91(%[[VALUE_n_35:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(2)), read<i32>(%[[VALUE_n_35]])), le<i32>(read<i32>(%[[VALUE_n_35]]), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1))))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_35]], sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_91]], read<i32>(%[[VALUE_n_35]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_92:[0-9]+]] @alloc_anti_range_92(%[[VALUE35:[0-9]+]] <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_92:[0-9]+]] @test_anti_range_92(%[[VALUE_n_36:[0-9]+]] n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), read<u32>(%[[VALUE_n_36]])), le<u32>(read<u32>(%[[VALUE_n_36]]), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_n_36]], sub<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_alloc_anti_range_92]], read<u32>(%[[VALUE_n_36]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_93:[0-9]+]] @alloc_anti_range_93(%[[VALUE36:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_93:[0-9]+]] @test_anti_range_93(%[[VALUE_n_37:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_37]])), le<i64>(read<i64>(%[[VALUE_n_37]]), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_37]], sub<i64, overflow=ub>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_93]], read<i64>(%[[VALUE_n_37]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_94:[0-9]+]] @alloc_anti_range_94(%[[VALUE37:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_94:[0-9]+]] @test_anti_range_94(%[[VALUE_n_38:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), read<u64>(%[[VALUE_n_38]])), le<u64>(read<u64>(%[[VALUE_n_38]]), sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_38]], sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_94]], read<u64>(%[[VALUE_n_38]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_95:[0-9]+]] @alloc_anti_range_95(%[[VALUE38:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_95:[0-9]+]] @test_anti_range_95(%[[VALUE_n_39:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_39]])), le<i64>(read<i64>(%[[VALUE_n_39]]), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_39]], sub<i64, overflow=ub>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_95]], read<i64>(%[[VALUE_n_39]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_96:[0-9]+]] @alloc_anti_range_96(%[[VALUE39:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_96:[0-9]+]] @test_anti_range_96(%[[VALUE_n_40:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), read<u64>(%[[VALUE_n_40]])), le<u64>(read<u64>(%[[VALUE_n_40]]), sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_40]], sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_96]], read<u64>(%[[VALUE_n_40]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_99:[0-9]+]] @alloc_anti_range_99(%[[VALUE40:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_99:[0-9]+]] @test_anti_range_99(%[[VALUE_n_41:[0-9]+]] n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_41]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_41]])), const<i32>(2)))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n_41]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_alloc_anti_range_99]], read<i8>(%[[VALUE_n_41]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_100:[0-9]+]] @alloc_anti_range_100(%[[VALUE41:[0-9]+]] <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_100:[0-9]+]] @test_anti_range_100(%[[VALUE_n_42:[0-9]+]] n: u8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_42]])))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_42]]))), const<i32>(2)))
// DEFAULT-NEXT:             write<u8>(%[[VALUE_n_42]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_alloc_anti_range_100]], read<u8>(%[[VALUE_n_42]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_101:[0-9]+]] @alloc_anti_range_101(%[[VALUE42:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_101:[0-9]+]] @test_anti_range_101(%[[VALUE_n_43:[0-9]+]] n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_43]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_43]])), const<i32>(2)))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_43]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_alloc_anti_range_101]], read<i16>(%[[VALUE_n_43]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_102:[0-9]+]] @alloc_anti_range_102(%[[VALUE43:[0-9]+]] <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_102:[0-9]+]] @test_anti_range_102(%[[VALUE_n_44:[0-9]+]] n: u16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_44]])))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_44]]))), const<i32>(2)))
// DEFAULT-NEXT:             write<u16>(%[[VALUE_n_44]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_alloc_anti_range_102]], read<u16>(%[[VALUE_n_44]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_103:[0-9]+]] @alloc_anti_range_103(%[[VALUE44:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_103:[0-9]+]] @test_anti_range_103(%[[VALUE_n_45:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), read<i32>(%[[VALUE_n_45]])), le<i32>(read<i32>(%[[VALUE_n_45]]), const<i32>(2)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_45]], sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_103]], read<i32>(%[[VALUE_n_45]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_104:[0-9]+]] @alloc_anti_range_104(%[[VALUE45:[0-9]+]] <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_104:[0-9]+]] @test_anti_range_104(%[[VALUE_n_46:[0-9]+]] n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)), read<u32>(%[[VALUE_n_46]])), le<u32>(read<u32>(%[[VALUE_n_46]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_n_46]], reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_alloc_anti_range_104]], read<u32>(%[[VALUE_n_46]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_105:[0-9]+]] @alloc_anti_range_105(%[[VALUE46:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_105:[0-9]+]] @test_anti_range_105(%[[VALUE_n_47:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(0)), read<i64>(%[[VALUE_n_47]])), le<i64>(read<i64>(%[[VALUE_n_47]]), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_47]], widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_105]], read<i64>(%[[VALUE_n_47]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_106:[0-9]+]] @alloc_anti_range_106(%[[VALUE47:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_106:[0-9]+]] @test_anti_range_106(%[[VALUE_n_48:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%[[VALUE_n_48]])), le<u64>(read<u64>(%[[VALUE_n_48]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_48]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_106]], read<u64>(%[[VALUE_n_48]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_107:[0-9]+]] @alloc_anti_range_107(%[[VALUE48:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_107:[0-9]+]] @test_anti_range_107(%[[VALUE_n_49:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(0)), read<i64>(%[[VALUE_n_49]])), le<i64>(read<i64>(%[[VALUE_n_49]]), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_49]], widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_107]], read<i64>(%[[VALUE_n_49]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_108:[0-9]+]] @alloc_anti_range_108(%[[VALUE49:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_108:[0-9]+]] @test_anti_range_108(%[[VALUE_n_50:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%[[VALUE_n_50]])), le<u64>(read<u64>(%[[VALUE_n_50]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_50]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_108]], read<u64>(%[[VALUE_n_50]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_111:[0-9]+]] @alloc_anti_range_111(%[[VALUE50:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_111:[0-9]+]] @test_anti_range_111(%[[VALUE_n_51:[0-9]+]] n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_51]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_51]])), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n_51]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_alloc_anti_range_111]], read<i8>(%[[VALUE_n_51]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_112:[0-9]+]] @alloc_anti_range_112(%[[VALUE51:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_112:[0-9]+]] @test_anti_range_112(%[[VALUE_n_52:[0-9]+]] n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_52]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_52]])), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_52]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_alloc_anti_range_112]], read<i16>(%[[VALUE_n_52]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_113:[0-9]+]] @alloc_anti_range_113(%[[VALUE52:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_113:[0-9]+]] @test_anti_range_113(%[[VALUE_n_53:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), read<i32>(%[[VALUE_n_53]])), le<i32>(read<i32>(%[[VALUE_n_53]]), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_53]], sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_113]], read<i32>(%[[VALUE_n_53]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_114:[0-9]+]] @alloc_anti_range_114(%[[VALUE53:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_114:[0-9]+]] @test_anti_range_114(%[[VALUE_n_54:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_54]])), le<i64>(read<i64>(%[[VALUE_n_54]]), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_54]], sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_114]], read<i64>(%[[VALUE_n_54]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_115:[0-9]+]] @alloc_anti_range_115(%[[VALUE54:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_115:[0-9]+]] @test_anti_range_115(%[[VALUE_n_55:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_55]])), le<i64>(read<i64>(%[[VALUE_n_55]]), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_55]], sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_115]], read<i64>(%[[VALUE_n_55]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_118:[0-9]+]] @alloc_anti_range_118(%[[VALUE55:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_118:[0-9]+]] @test_anti_range_118(%[[VALUE_n_56:[0-9]+]] n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_56]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_56]])), const<i32>(0)))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n_56]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_alloc_anti_range_118]], read<i8>(%[[VALUE_n_56]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_119:[0-9]+]] @alloc_anti_range_119(%[[VALUE56:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_119:[0-9]+]] @test_anti_range_119(%[[VALUE_n_57:[0-9]+]] n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_57]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_57]])), const<i32>(0)))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_57]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_alloc_anti_range_119]], read<i16>(%[[VALUE_n_57]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_120:[0-9]+]] @alloc_anti_range_120(%[[VALUE57:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_120:[0-9]+]] @test_anti_range_120(%[[VALUE_n_58:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), read<i32>(%[[VALUE_n_58]])), le<i32>(read<i32>(%[[VALUE_n_58]]), const<i32>(0)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_58]], sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_120]], read<i32>(%[[VALUE_n_58]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_121:[0-9]+]] @alloc_anti_range_121(%[[VALUE58:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_121:[0-9]+]] @test_anti_range_121(%[[VALUE_n_59:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_59]])), le<i64>(read<i64>(%[[VALUE_n_59]]), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_59]], sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_121]], read<i64>(%[[VALUE_n_59]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_122:[0-9]+]] @alloc_anti_range_122(%[[VALUE59:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_122:[0-9]+]] @test_anti_range_122(%[[VALUE_n_60:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_60]])), le<i64>(read<i64>(%[[VALUE_n_60]]), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_60]], sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_122]], read<i64>(%[[VALUE_n_60]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_125:[0-9]+]] @alloc_anti_range_125(%[[VALUE60:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_125:[0-9]+]] @test_anti_range_125(%[[VALUE_n_61:[0-9]+]] n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_61]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_61]])), const<i32>(1)))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n_61]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_alloc_anti_range_125]], read<i8>(%[[VALUE_n_61]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_126:[0-9]+]] @alloc_anti_range_126(%[[VALUE61:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_126:[0-9]+]] @test_anti_range_126(%[[VALUE_n_62:[0-9]+]] n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_62]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_62]])), const<i32>(1)))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_62]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_alloc_anti_range_126]], read<i16>(%[[VALUE_n_62]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_127:[0-9]+]] @alloc_anti_range_127(%[[VALUE62:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_127:[0-9]+]] @test_anti_range_127(%[[VALUE_n_63:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), read<i32>(%[[VALUE_n_63]])), le<i32>(read<i32>(%[[VALUE_n_63]]), const<i32>(1)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_63]], sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_127]], read<i32>(%[[VALUE_n_63]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_128:[0-9]+]] @alloc_anti_range_128(%[[VALUE63:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_128:[0-9]+]] @test_anti_range_128(%[[VALUE_n_64:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_64]])), le<i64>(read<i64>(%[[VALUE_n_64]]), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_64]], sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_128]], read<i64>(%[[VALUE_n_64]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_129:[0-9]+]] @alloc_anti_range_129(%[[VALUE64:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_129:[0-9]+]] @test_anti_range_129(%[[VALUE_n_65:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_65]])), le<i64>(read<i64>(%[[VALUE_n_65]]), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_65]], sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_129]], read<i64>(%[[VALUE_n_65]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_132:[0-9]+]] @alloc_anti_range_132(%[[VALUE65:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_132:[0-9]+]] @test_anti_range_132(%[[VALUE_n_66:[0-9]+]] n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_66]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_66]])), const<i32>(2)))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n_66]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_alloc_anti_range_132]], read<i8>(%[[VALUE_n_66]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_133:[0-9]+]] @alloc_anti_range_133(%[[VALUE66:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_133:[0-9]+]] @test_anti_range_133(%[[VALUE_n_67:[0-9]+]] n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_67]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_67]])), const<i32>(2)))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_67]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_alloc_anti_range_133]], read<i16>(%[[VALUE_n_67]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_134:[0-9]+]] @alloc_anti_range_134(%[[VALUE67:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_134:[0-9]+]] @test_anti_range_134(%[[VALUE_n_68:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), read<i32>(%[[VALUE_n_68]])), le<i32>(read<i32>(%[[VALUE_n_68]]), const<i32>(2)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_68]], sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_134]], read<i32>(%[[VALUE_n_68]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_135:[0-9]+]] @alloc_anti_range_135(%[[VALUE68:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_135:[0-9]+]] @test_anti_range_135(%[[VALUE_n_69:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_69]])), le<i64>(read<i64>(%[[VALUE_n_69]]), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_69]], sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_135]], read<i64>(%[[VALUE_n_69]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_136:[0-9]+]] @alloc_anti_range_136(%[[VALUE69:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_136:[0-9]+]] @test_anti_range_136(%[[VALUE_n_70:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_70]])), le<i64>(read<i64>(%[[VALUE_n_70]]), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_70]], sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_136]], read<i64>(%[[VALUE_n_70]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_139:[0-9]+]] @alloc_anti_range_139(%[[VALUE70:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_139:[0-9]+]] @test_anti_range_139(%[[VALUE_n_71:[0-9]+]] n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_71]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_71]])), const<i32>(2)))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n_71]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_alloc_anti_range_139]], read<i8>(%[[VALUE_n_71]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_140:[0-9]+]] @alloc_anti_range_140(%[[VALUE71:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_140:[0-9]+]] @test_anti_range_140(%[[VALUE_n_72:[0-9]+]] n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_72]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_72]])), const<i32>(2)))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_72]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_alloc_anti_range_140]], read<i16>(%[[VALUE_n_72]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_141:[0-9]+]] @alloc_anti_range_141(%[[VALUE72:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_141:[0-9]+]] @test_anti_range_141(%[[VALUE_n_73:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(1)), read<i32>(%[[VALUE_n_73]])), le<i32>(read<i32>(%[[VALUE_n_73]]), const<i32>(2)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_73]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_141]], read<i32>(%[[VALUE_n_73]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_142:[0-9]+]] @alloc_anti_range_142(%[[VALUE73:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_142:[0-9]+]] @test_anti_range_142(%[[VALUE_n_74:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))), read<i64>(%[[VALUE_n_74]])), le<i64>(read<i64>(%[[VALUE_n_74]]), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_74]], widen<i64, reason=assign>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_142]], read<i64>(%[[VALUE_n_74]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_143:[0-9]+]] @alloc_anti_range_143(%[[VALUE74:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_143:[0-9]+]] @test_anti_range_143(%[[VALUE_n_75:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(1)), read<i64>(%[[VALUE_n_75]])), le<i64>(read<i64>(%[[VALUE_n_75]]), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_75]], widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_143]], read<i64>(%[[VALUE_n_75]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
