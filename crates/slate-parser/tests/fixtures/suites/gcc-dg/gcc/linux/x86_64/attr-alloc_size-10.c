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
// DEFAULT-NEXT:     type @type0 ptrdiff_t = i64;
// DEFAULT-NEXT:     type @type1 size_t = u64;
// DEFAULT-NEXT:     type @type2 dummy = void;
// DEFAULT-NEXT:     type @type3 dummy = void;
// DEFAULT-NEXT:     type @type4 dummy = void;
// DEFAULT-NEXT:     type @type5 dummy = void;
// DEFAULT-NEXT:     type @type6 dummy = void;
// DEFAULT-NEXT:     type @type7 dummy = void;
// DEFAULT-NEXT:     type @type8 dummy = void;
// DEFAULT-NEXT:     type @type9 dummy = void;
// DEFAULT-NEXT:     type @type10 dummy = void;
// DEFAULT-NEXT:     type @type11 dummy = void;
// DEFAULT-NEXT:     type @type12 dummy = void;
// DEFAULT-NEXT:     type @type13 dummy = void;
// DEFAULT-NEXT:     type @type14 dummy = void;
// DEFAULT-NEXT:     type @type15 dummy = void;
// DEFAULT-NEXT:     type @type16 dummy = void;
// DEFAULT-NEXT:     type @type17 dummy = void;
// DEFAULT-NEXT:     type @type18 dummy = void;
// DEFAULT-NEXT:     type @type19 dummy = void;
// DEFAULT-NEXT:     type @type20 dummy = void;
// DEFAULT-NEXT:     type @type21 dummy = void;
// DEFAULT-NEXT:     type @type22 dummy = void;
// DEFAULT-NEXT:     type @type23 dummy = void;
// DEFAULT-NEXT:     type @type24 dummy = void;
// DEFAULT-NEXT:     type @type25 dummy = void;
// DEFAULT-NEXT:     type @type26 dummy = void;
// DEFAULT-NEXT:     type @type27 dummy = void;
// DEFAULT-NEXT:     type @type28 dummy = void;
// DEFAULT-NEXT:     type @type29 dummy = void;
// DEFAULT-NEXT:     type @type30 dummy = void;
// DEFAULT-NEXT:     type @type31 dummy = void;
// DEFAULT-NEXT:     type @type32 dummy = void;
// DEFAULT-NEXT:     type @type33 dummy = void;
// DEFAULT-NEXT:     type @type34 dummy = void;
// DEFAULT-NEXT:     type @type35 dummy = void;
// DEFAULT-NEXT:     type @type36 dummy = void;
// DEFAULT-NEXT:     type @type37 dummy = void;
// DEFAULT-NEXT:     type @type38 dummy = void;
// DEFAULT-NEXT:     type @type39 dummy = void;
// DEFAULT-NEXT:     type @type40 dummy = void;
// DEFAULT-NEXT:     type @type41 dummy = void;
// DEFAULT-NEXT:     type @type42 dummy = void;
// DEFAULT-NEXT:     type @type43 dummy = void;
// DEFAULT-NEXT:     type @type44 dummy = void;
// DEFAULT-NEXT:     type @type45 dummy = void;
// DEFAULT-NEXT:     type @type46 dummy = void;
// DEFAULT-NEXT:     type @type47 dummy = void;
// DEFAULT-NEXT:     type @type48 dummy = void;
// DEFAULT-NEXT:     type @type49 dummy = void;
// DEFAULT-NEXT:     type @type50 dummy = void;
// DEFAULT-NEXT:     type @type51 dummy = void;
// DEFAULT-NEXT:     type @type52 dummy = void;
// DEFAULT-NEXT:     type @type53 dummy = void;
// DEFAULT-NEXT:     type @type54 dummy = void;
// DEFAULT-NEXT:     type @type55 dummy = void;
// DEFAULT-NEXT:     type @type56 dummy = void;
// DEFAULT-NEXT:     type @type57 dummy = void;
// DEFAULT-NEXT:     type @type58 dummy = void;
// DEFAULT-NEXT:     type @type59 dummy = void;
// DEFAULT-NEXT:     type @type60 dummy = void;
// DEFAULT-NEXT:     type @type61 dummy = void;
// DEFAULT-NEXT:     type @type62 dummy = void;
// DEFAULT-NEXT:     type @type63 dummy = void;
// DEFAULT-NEXT:     type @type64 dummy = void;
// DEFAULT-NEXT:     type @type65 dummy = void;
// DEFAULT-NEXT:     type @type66 dummy = void;
// DEFAULT-NEXT:     type @type67 dummy = void;
// DEFAULT-NEXT:     type @type68 dummy = void;
// DEFAULT-NEXT:     type @type69 dummy = void;
// DEFAULT-NEXT:     type @type70 dummy = void;
// DEFAULT-NEXT:     type @type71 dummy = void;
// DEFAULT-NEXT:     type @type72 dummy = void;
// DEFAULT-NEXT:     type @type73 dummy = void;
// DEFAULT-NEXT:     type @type74 dummy = void;
// DEFAULT-NEXT:     type @type75 dummy = void;
// DEFAULT-NEXT:     type @type76 dummy = void;
// DEFAULT-NEXT:     fn %4 @alloc_anti_range_50(%228 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @test_anti_range_50(%3 n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%3))), le<i32>(widen<i32, reason=promotion>(read<i8>(%3)), sub<i32, overflow=ub>(const<i32>(127), const<i32>(1))))
// DEFAULT-NEXT:             write<i8>(%3, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, read<i8>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @alloc_anti_range_51(%229 <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @test_anti_range_51(%7 n: u8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%7)))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%7))), sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1))))
// DEFAULT-NEXT:             write<u8>(%7, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1)), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u8) -> ptr<void>>(%8, read<u8>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @alloc_anti_range_52(%230 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %9 @test_anti_range_52(%10 n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1)), widen<i32, reason=promotion>(read<i16>(%10))), le<i32>(widen<i32, reason=promotion>(read<i16>(%10)), sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1))))
// DEFAULT-NEXT:             write<i16>(%10, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%11, read<i16>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @alloc_anti_range_53(%231 <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %12 @test_anti_range_53(%13 n: u16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%13))))), le<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%13)))), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u16>(%13, truncate<u16, reason=assign, fits=unknown>(sub<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u16) -> ptr<void>>(%14, read<u16>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @alloc_anti_range_54(%232 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %15 @test_anti_range_54(%16 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1)), read<i32>(%16)), le<i32>(read<i32>(%16), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1))))
// DEFAULT-NEXT:             write<i32>(%16, sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%17, read<i32>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @alloc_anti_range_55(%233 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %18 @test_anti_range_55(%19 n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), read<u32>(%19)), le<u32>(read<u32>(%19), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%19, sub<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%20, read<u32>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @alloc_anti_range_56(%234 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %21 @test_anti_range_56(%22 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))), read<i64>(%22)), le<i64>(read<i64>(%22), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%22, sub<i64, overflow=ub>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%23, read<i64>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @alloc_anti_range_57(%235 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %24 @test_anti_range_57(%25 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%25)), le<u64>(read<u64>(%25), sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%25, sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%26, read<u64>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @alloc_anti_range_58(%236 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %27 @test_anti_range_58(%28 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))), read<i64>(%28)), le<i64>(read<i64>(%28), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%28, sub<i64, overflow=ub>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%29, read<i64>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @alloc_anti_range_59(%237 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %30 @test_anti_range_59(%31 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%31)), le<u64>(read<u64>(%31), sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%31, sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%32, read<u64>(%31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @alloc_anti_range_62(%238 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %33 @test_anti_range_62(%34 n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), widen<i32, reason=promotion>(read<i8>(%34))), le<i32>(widen<i32, reason=promotion>(read<i8>(%34)), const<i32>(0)))
// DEFAULT-NEXT:             write<i8>(%34, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%35, read<i8>(%34));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @alloc_anti_range_63(%239 <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %36 @test_anti_range_63(%37 n: u8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%37)))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%37))), const<i32>(0)))
// DEFAULT-NEXT:             write<u8>(%37, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u8) -> ptr<void>>(%38, read<u8>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @alloc_anti_range_64(%240 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %39 @test_anti_range_64(%40 n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), widen<i32, reason=promotion>(read<i16>(%40))), le<i32>(widen<i32, reason=promotion>(read<i16>(%40)), const<i32>(0)))
// DEFAULT-NEXT:             write<i16>(%40, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%41, read<i16>(%40));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @alloc_anti_range_65(%241 <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %42 @test_anti_range_65(%43 n: u16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%43)))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%43))), const<i32>(0)))
// DEFAULT-NEXT:             write<u16>(%43, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u16) -> ptr<void>>(%44, read<u16>(%43));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @alloc_anti_range_66(%242 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %45 @test_anti_range_66(%46 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), read<i32>(%46)), le<i32>(read<i32>(%46), const<i32>(0)))
// DEFAULT-NEXT:             write<i32>(%46, sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%47, read<i32>(%46));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @alloc_anti_range_67(%243 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %48 @test_anti_range_67(%49 n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)), read<u32>(%49)), le<u32>(read<u32>(%49), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             write<u32>(%49, reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%50, read<u32>(%49));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @alloc_anti_range_68(%244 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %51 @test_anti_range_68(%52 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(0)), read<i64>(%52)), le<i64>(read<i64>(%52), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<i64>(%52, widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%53, read<i64>(%52));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @alloc_anti_range_69(%245 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %54 @test_anti_range_69(%55 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%55)), le<u64>(read<u64>(%55), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:             write<u64>(%55, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%56, read<u64>(%55));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %59 @alloc_anti_range_70(%246 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %57 @test_anti_range_70(%58 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(0)), read<i64>(%58)), le<i64>(read<i64>(%58), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<i64>(%58, widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%59, read<i64>(%58));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @alloc_anti_range_71(%247 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %60 @test_anti_range_71(%61 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%61)), le<u64>(read<u64>(%61), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:             write<u64>(%61, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%62, read<u64>(%61));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %65 @alloc_anti_range_74(%248 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %63 @test_anti_range_74(%64 n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), widen<i32, reason=promotion>(read<i8>(%64))), le<i32>(widen<i32, reason=promotion>(read<i8>(%64)), const<i32>(1)))
// DEFAULT-NEXT:             write<i8>(%64, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%65, read<i8>(%64));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @alloc_anti_range_75(%249 <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %66 @test_anti_range_75(%67 n: u8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%67)))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%67))), const<i32>(1)))
// DEFAULT-NEXT:             write<u8>(%67, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u8) -> ptr<void>>(%68, read<u8>(%67));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %71 @alloc_anti_range_76(%250 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %69 @test_anti_range_76(%70 n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), widen<i32, reason=promotion>(read<i16>(%70))), le<i32>(widen<i32, reason=promotion>(read<i16>(%70)), const<i32>(1)))
// DEFAULT-NEXT:             write<i16>(%70, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%71, read<i16>(%70));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @alloc_anti_range_77(%251 <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %72 @test_anti_range_77(%73 n: u16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%73)))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%73))), const<i32>(1)))
// DEFAULT-NEXT:             write<u16>(%73, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u16) -> ptr<void>>(%74, read<u16>(%73));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %77 @alloc_anti_range_78(%252 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %75 @test_anti_range_78(%76 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), read<i32>(%76)), le<i32>(read<i32>(%76), const<i32>(1)))
// DEFAULT-NEXT:             write<i32>(%76, sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%77, read<i32>(%76));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @alloc_anti_range_79(%253 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %78 @test_anti_range_79(%79 n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), read<u32>(%79)), le<u32>(read<u32>(%79), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             write<u32>(%79, reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%80, read<u32>(%79));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %83 @alloc_anti_range_80(%254 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %81 @test_anti_range_80(%82 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(1)), read<i64>(%82)), le<i64>(read<i64>(%82), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%82, widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%83, read<i64>(%82));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %86 @alloc_anti_range_81(%255 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %84 @test_anti_range_81(%85 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), read<u64>(%85)), le<u64>(read<u64>(%85), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u64>(%85, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%86, read<u64>(%85));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %89 @alloc_anti_range_82(%256 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %87 @test_anti_range_82(%88 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(1)), read<i64>(%88)), le<i64>(read<i64>(%88), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%88, widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%89, read<i64>(%88));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %92 @alloc_anti_range_83(%257 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %90 @test_anti_range_83(%91 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), read<u64>(%91)), le<u64>(read<u64>(%91), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u64>(%91, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%92, read<u64>(%91));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %95 @alloc_anti_range_87(%258 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %93 @test_anti_range_87(%94 n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%94))), le<i32>(widen<i32, reason=promotion>(read<i8>(%94)), sub<i32, overflow=ub>(const<i32>(127), const<i32>(1))))
// DEFAULT-NEXT:             write<i8>(%94, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%95, read<i8>(%94));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %98 @alloc_anti_range_88(%259 <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %96 @test_anti_range_88(%97 n: u8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(2)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%97)))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%97))), sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1))))
// DEFAULT-NEXT:             write<u8>(%97, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(2)), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u8) -> ptr<void>>(%98, read<u8>(%97));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %101 @alloc_anti_range_89(%260 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %99 @test_anti_range_89(%100 n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%100))), le<i32>(widen<i32, reason=promotion>(read<i16>(%100)), sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1))))
// DEFAULT-NEXT:             write<i16>(%100, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%101, read<i16>(%100));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %104 @alloc_anti_range_90(%261 <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %102 @test_anti_range_90(%103 n: u16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%103))))), le<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%103)))), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u16>(%103, truncate<u16, reason=assign, fits=unknown>(sub<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u16) -> ptr<void>>(%104, read<u16>(%103));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %107 @alloc_anti_range_91(%262 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %105 @test_anti_range_91(%106 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(2)), read<i32>(%106)), le<i32>(read<i32>(%106), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1))))
// DEFAULT-NEXT:             write<i32>(%106, sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%107, read<i32>(%106));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %110 @alloc_anti_range_92(%263 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %108 @test_anti_range_92(%109 n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), read<u32>(%109)), le<u32>(read<u32>(%109), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%109, sub<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%110, read<u32>(%109));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %113 @alloc_anti_range_93(%264 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %111 @test_anti_range_93(%112 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%112)), le<i64>(read<i64>(%112), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%112, sub<i64, overflow=ub>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%113, read<i64>(%112));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %116 @alloc_anti_range_94(%265 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %114 @test_anti_range_94(%115 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), read<u64>(%115)), le<u64>(read<u64>(%115), sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%115, sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%116, read<u64>(%115));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %119 @alloc_anti_range_95(%266 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %117 @test_anti_range_95(%118 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%118)), le<i64>(read<i64>(%118), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%118, sub<i64, overflow=ub>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%119, read<i64>(%118));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %122 @alloc_anti_range_96(%267 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %120 @test_anti_range_96(%121 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), read<u64>(%121)), le<u64>(read<u64>(%121), sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%121, sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%122, read<u64>(%121));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %125 @alloc_anti_range_99(%268 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %123 @test_anti_range_99(%124 n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), widen<i32, reason=promotion>(read<i8>(%124))), le<i32>(widen<i32, reason=promotion>(read<i8>(%124)), const<i32>(2)))
// DEFAULT-NEXT:             write<i8>(%124, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%125, read<i8>(%124));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %128 @alloc_anti_range_100(%269 <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %126 @test_anti_range_100(%127 n: u8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%127)))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%127))), const<i32>(2)))
// DEFAULT-NEXT:             write<u8>(%127, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u8) -> ptr<void>>(%128, read<u8>(%127));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %131 @alloc_anti_range_101(%270 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %129 @test_anti_range_101(%130 n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), widen<i32, reason=promotion>(read<i16>(%130))), le<i32>(widen<i32, reason=promotion>(read<i16>(%130)), const<i32>(2)))
// DEFAULT-NEXT:             write<i16>(%130, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%131, read<i16>(%130));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %134 @alloc_anti_range_102(%271 <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %132 @test_anti_range_102(%133 n: u16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%133)))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%133))), const<i32>(2)))
// DEFAULT-NEXT:             write<u16>(%133, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u16) -> ptr<void>>(%134, read<u16>(%133));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %137 @alloc_anti_range_103(%272 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %135 @test_anti_range_103(%136 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), read<i32>(%136)), le<i32>(read<i32>(%136), const<i32>(2)))
// DEFAULT-NEXT:             write<i32>(%136, sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%137, read<i32>(%136));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %140 @alloc_anti_range_104(%273 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %138 @test_anti_range_104(%139 n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)), read<u32>(%139)), le<u32>(read<u32>(%139), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))))
// DEFAULT-NEXT:             write<u32>(%139, reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%140, read<u32>(%139));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %143 @alloc_anti_range_105(%274 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %141 @test_anti_range_105(%142 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(0)), read<i64>(%142)), le<i64>(read<i64>(%142), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%142, widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%143, read<i64>(%142));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %146 @alloc_anti_range_106(%275 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %144 @test_anti_range_106(%145 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%145)), le<u64>(read<u64>(%145), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             write<u64>(%145, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%146, read<u64>(%145));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %149 @alloc_anti_range_107(%276 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %147 @test_anti_range_107(%148 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(0)), read<i64>(%148)), le<i64>(read<i64>(%148), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%148, widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%149, read<i64>(%148));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %152 @alloc_anti_range_108(%277 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %150 @test_anti_range_108(%151 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%151)), le<u64>(read<u64>(%151), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             write<u64>(%151, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%152, read<u64>(%151));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %155 @alloc_anti_range_111(%278 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %153 @test_anti_range_111(%154 n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%154))), le<i32>(widen<i32, reason=promotion>(read<i8>(%154)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<i8>(%154, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%155, read<i8>(%154));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %158 @alloc_anti_range_112(%279 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %156 @test_anti_range_112(%157 n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%157))), le<i32>(widen<i32, reason=promotion>(read<i16>(%157)), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<i16>(%157, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%158, read<i16>(%157));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %161 @alloc_anti_range_113(%280 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %159 @test_anti_range_113(%160 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), read<i32>(%160)), le<i32>(read<i32>(%160), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<i32>(%160, sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%161, read<i32>(%160));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %164 @alloc_anti_range_114(%281 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %162 @test_anti_range_114(%163 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%163)), le<i64>(read<i64>(%163), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%163, sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%164, read<i64>(%163));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %167 @alloc_anti_range_115(%282 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %165 @test_anti_range_115(%166 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%166)), le<i64>(read<i64>(%166), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%166, sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%167, read<i64>(%166));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %170 @alloc_anti_range_118(%283 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %168 @test_anti_range_118(%169 n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%169))), le<i32>(widen<i32, reason=promotion>(read<i8>(%169)), const<i32>(0)))
// DEFAULT-NEXT:             write<i8>(%169, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%170, read<i8>(%169));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %173 @alloc_anti_range_119(%284 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %171 @test_anti_range_119(%172 n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%172))), le<i32>(widen<i32, reason=promotion>(read<i16>(%172)), const<i32>(0)))
// DEFAULT-NEXT:             write<i16>(%172, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%173, read<i16>(%172));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %176 @alloc_anti_range_120(%285 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %174 @test_anti_range_120(%175 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), read<i32>(%175)), le<i32>(read<i32>(%175), const<i32>(0)))
// DEFAULT-NEXT:             write<i32>(%175, sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%176, read<i32>(%175));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %179 @alloc_anti_range_121(%286 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %177 @test_anti_range_121(%178 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%178)), le<i64>(read<i64>(%178), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<i64>(%178, sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%179, read<i64>(%178));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %182 @alloc_anti_range_122(%287 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %180 @test_anti_range_122(%181 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%181)), le<i64>(read<i64>(%181), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<i64>(%181, sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%182, read<i64>(%181));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %185 @alloc_anti_range_125(%288 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %183 @test_anti_range_125(%184 n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%184))), le<i32>(widen<i32, reason=promotion>(read<i8>(%184)), const<i32>(1)))
// DEFAULT-NEXT:             write<i8>(%184, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%185, read<i8>(%184));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %188 @alloc_anti_range_126(%289 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %186 @test_anti_range_126(%187 n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%187))), le<i32>(widen<i32, reason=promotion>(read<i16>(%187)), const<i32>(1)))
// DEFAULT-NEXT:             write<i16>(%187, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%188, read<i16>(%187));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %191 @alloc_anti_range_127(%290 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %189 @test_anti_range_127(%190 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), read<i32>(%190)), le<i32>(read<i32>(%190), const<i32>(1)))
// DEFAULT-NEXT:             write<i32>(%190, sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%191, read<i32>(%190));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %194 @alloc_anti_range_128(%291 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %192 @test_anti_range_128(%193 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%193)), le<i64>(read<i64>(%193), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%193, sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%194, read<i64>(%193));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %197 @alloc_anti_range_129(%292 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %195 @test_anti_range_129(%196 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%196)), le<i64>(read<i64>(%196), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%196, sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%197, read<i64>(%196));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %200 @alloc_anti_range_132(%293 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %198 @test_anti_range_132(%199 n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%199))), le<i32>(widen<i32, reason=promotion>(read<i8>(%199)), const<i32>(2)))
// DEFAULT-NEXT:             write<i8>(%199, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%200, read<i8>(%199));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %203 @alloc_anti_range_133(%294 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %201 @test_anti_range_133(%202 n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%202))), le<i32>(widen<i32, reason=promotion>(read<i16>(%202)), const<i32>(2)))
// DEFAULT-NEXT:             write<i16>(%202, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%203, read<i16>(%202));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %206 @alloc_anti_range_134(%295 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %204 @test_anti_range_134(%205 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), read<i32>(%205)), le<i32>(read<i32>(%205), const<i32>(2)))
// DEFAULT-NEXT:             write<i32>(%205, sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%206, read<i32>(%205));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %209 @alloc_anti_range_135(%296 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %207 @test_anti_range_135(%208 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%208)), le<i64>(read<i64>(%208), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%208, sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%209, read<i64>(%208));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %212 @alloc_anti_range_136(%297 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %210 @test_anti_range_136(%211 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%211)), le<i64>(read<i64>(%211), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%211, sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%212, read<i64>(%211));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %215 @alloc_anti_range_139(%298 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %213 @test_anti_range_139(%214 n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%214))), le<i32>(widen<i32, reason=promotion>(read<i8>(%214)), const<i32>(2)))
// DEFAULT-NEXT:             write<i8>(%214, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%215, read<i8>(%214));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %218 @alloc_anti_range_140(%299 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %216 @test_anti_range_140(%217 n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i16>(%217))), le<i32>(widen<i32, reason=promotion>(read<i16>(%217)), const<i32>(2)))
// DEFAULT-NEXT:             write<i16>(%217, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%218, read<i16>(%217));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %221 @alloc_anti_range_141(%300 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %219 @test_anti_range_141(%220 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(1)), read<i32>(%220)), le<i32>(read<i32>(%220), const<i32>(2)))
// DEFAULT-NEXT:             write<i32>(%220, sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%221, read<i32>(%220));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %224 @alloc_anti_range_142(%301 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %222 @test_anti_range_142(%223 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))), read<i64>(%223)), le<i64>(read<i64>(%223), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%223, widen<i64, reason=assign>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%224, read<i64>(%223));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %227 @alloc_anti_range_143(%302 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %225 @test_anti_range_143(%226 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(widen<i64, reason=usual_arith>(const<i32>(1)), read<i64>(%226)), le<i64>(read<i64>(%226), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             write<i64>(%226, widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%227, read<i64>(%226));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
