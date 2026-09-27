/* Verify that -Walloc-size-greater-than doesn't cause false positives
   for anti-ranges.  Note that not all of the statements below result
   in the argument being represented as an anti-range.

   { dg-do compile }
   { dg-options "-O2 -Walloc-size-larger-than=12 -ftrack-macro-expansion=0" } */

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

#define ALLOC_MAX   12

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

/* The following tests fail because of missing range information.  The xfail
   exclusions are PR79356.  */
TEST (signed char, SCHAR_MIN + 2, ALLOC_MAX);   /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" "missing range info for signed char" } */
TEST (short, SHRT_MIN + 2, ALLOC_MAX); /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" "missing range info for short" } */
TEST (int, INT_MIN + 2, ALLOC_MAX);    /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" } */
TEST (int, -3, ALLOC_MAX);             /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" } */
TEST (int, -2, ALLOC_MAX);             /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" } */
TEST (int, -1, ALLOC_MAX);             /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" } */
TEST (int,  0, ALLOC_MAX);             /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" } */
TEST (int,  1, ALLOC_MAX);             /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" } */
TEST (int,  1, INT_MAX - 1);           /* { dg-warning "argument 1 range \\\[\[0-9\]+, \[0-9\]+\\\] exceeds maximum object size 12" } */

/* The following two aren't necessarily anti-ranges.  */
TEST (int,  1, INT_MAX);               /* { dg-warning "argument 1 range \\\[-\[0-9\]+, 0\\\] is negative" } */
TEST (int,  0, INT_MAX);               /* { dg-warning "argument 1 range \\\[-\[0-9\]+, -1\\\] is negative" } */

TEST (long, LONG_MIN + 2, ALLOC_MAX);  /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" } */
TEST (ptrdiff_t, PTRDIFF_MIN + 2, ALLOC_MAX);  /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" } */

TEST (unsigned, 0, ALLOC_MAX);         /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" } */
TEST (unsigned long, 0, ALLOC_MAX);    /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" } */
TEST (size_t, 0, ALLOC_MAX);           /* { dg-warning "argument 1 range \\\[13, \[0-9\]+\\\] exceeds maximum object size 12" } */

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %4 @alloc_anti_range_50(%51 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @test_anti_range_50(%3 n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%3))), le<i32>(widen<i32, reason=promotion>(read<i8>(%3)), const<i32>(12)))
// DEFAULT-NEXT:             write<i8>(%3, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, read<i8>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @alloc_anti_range_51(%52 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @test_anti_range_51(%7 n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%7))), le<i32>(widen<i32, reason=promotion>(read<i16>(%7)), const<i32>(12)))
// DEFAULT-NEXT:             write<i16>(%7, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, read<i16>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @alloc_anti_range_52(%53 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %9 @test_anti_range_52(%10 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), read<i32>(%10)), le<i32>(read<i32>(%10), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%10, sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%11, read<i32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @alloc_anti_range_53(%54 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %12 @test_anti_range_53(%13 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(3)), read<i32>(%13)), le<i32>(read<i32>(%13), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%13, sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(3)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%14, read<i32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @alloc_anti_range_54(%55 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %15 @test_anti_range_54(%16 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(2)), read<i32>(%16)), le<i32>(read<i32>(%16), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%16, sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%17, read<i32>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @alloc_anti_range_55(%56 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %18 @test_anti_range_55(%19 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(1)), read<i32>(%19)), le<i32>(read<i32>(%19), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%19, sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%20, read<i32>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @alloc_anti_range_56(%57 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %21 @test_anti_range_56(%22 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), read<i32>(%22)), le<i32>(read<i32>(%22), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%22, sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%23, read<i32>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @alloc_anti_range_57(%58 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %24 @test_anti_range_57(%25 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), read<i32>(%25)), le<i32>(read<i32>(%25), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%25, sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%26, read<i32>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @alloc_anti_range_58(%59 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %27 @test_anti_range_58(%28 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), read<i32>(%28)), le<i32>(read<i32>(%28), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1))))
// DEFAULT-NEXT:             write<i32>(%28, sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%29, read<i32>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @alloc_anti_range_61(%60 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %30 @test_anti_range_61(%31 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), read<i32>(%31)), le<i32>(read<i32>(%31), const<i32>(2147483647)))
// DEFAULT-NEXT:             write<i32>(%31, sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%32, read<i32>(%31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @alloc_anti_range_62(%61 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %33 @test_anti_range_62(%34 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), read<i32>(%34)), le<i32>(read<i32>(%34), const<i32>(2147483647)))
// DEFAULT-NEXT:             write<i32>(%34, sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%35, read<i32>(%34));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @alloc_anti_range_64(%62 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %36 @test_anti_range_64(%37 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%37)), le<i64>(read<i64>(%37), widen<i64, reason=usual_arith>(const<i32>(12))))
// DEFAULT-NEXT:             write<i64>(%37, sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%38, read<i64>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @alloc_anti_range_65(%63 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %39 @test_anti_range_65(%40 n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%40)), le<i64>(read<i64>(%40), widen<i64, reason=usual_arith>(const<i32>(12))))
// DEFAULT-NEXT:             write<i64>(%40, sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%41, read<i64>(%40));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @alloc_anti_range_67(%64 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %42 @test_anti_range_67(%43 n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)), read<u32>(%43)), le<u32>(read<u32>(%43), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(12))))
// DEFAULT-NEXT:             write<u32>(%43, reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%44, read<u32>(%43));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @alloc_anti_range_68(%65 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %45 @test_anti_range_68(%46 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%46)), le<u64>(read<u64>(%46), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12)))))
// DEFAULT-NEXT:             write<u64>(%46, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%47, read<u64>(%46));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @alloc_anti_range_69(%66 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %48 @test_anti_range_69(%49 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%49)), le<u64>(read<u64>(%49), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12)))))
// DEFAULT-NEXT:             write<u64>(%49, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%50, read<u64>(%49));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
