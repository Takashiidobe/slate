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
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_50:[0-9]+]] @alloc_anti_range_50(%[[VALUE0:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_50:[0-9]+]] @test_anti_range_50(%[[VALUE_n:[0-9]+]] n: i8) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n]])), const<i32>(12)))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_alloc_anti_range_50]], read<i8>(%[[VALUE_n]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_51:[0-9]+]] @alloc_anti_range_51(%[[VALUE1:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_51:[0-9]+]] @test_anti_range_51(%[[VALUE_n_2:[0-9]+]] n: i16) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_2]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_2]])), const<i32>(12)))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_2]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_alloc_anti_range_51]], read<i16>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_52:[0-9]+]] @alloc_anti_range_52(%[[VALUE2:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_52:[0-9]+]] @test_anti_range_52(%[[VALUE_n_3:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), read<i32>(%[[VALUE_n_3]])), le<i32>(read<i32>(%[[VALUE_n_3]]), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_3]], sub<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_52]], read<i32>(%[[VALUE_n_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_53:[0-9]+]] @alloc_anti_range_53(%[[VALUE3:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_53:[0-9]+]] @test_anti_range_53(%[[VALUE_n_4:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(3)), read<i32>(%[[VALUE_n_4]])), le<i32>(read<i32>(%[[VALUE_n_4]]), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_4]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(3)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_53]], read<i32>(%[[VALUE_n_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_54:[0-9]+]] @alloc_anti_range_54(%[[VALUE4:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_54:[0-9]+]] @test_anti_range_54(%[[VALUE_n_5:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(2)), read<i32>(%[[VALUE_n_5]])), le<i32>(read<i32>(%[[VALUE_n_5]]), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_5]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_54]], read<i32>(%[[VALUE_n_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_55:[0-9]+]] @alloc_anti_range_55(%[[VALUE5:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_55:[0-9]+]] @test_anti_range_55(%[[VALUE_n_6:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(neg<i32, overflow=ub>(const<i32>(1)), read<i32>(%[[VALUE_n_6]])), le<i32>(read<i32>(%[[VALUE_n_6]]), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_6]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_55]], read<i32>(%[[VALUE_n_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_56:[0-9]+]] @alloc_anti_range_56(%[[VALUE6:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_56:[0-9]+]] @test_anti_range_56(%[[VALUE_n_7:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), read<i32>(%[[VALUE_n_7]])), le<i32>(read<i32>(%[[VALUE_n_7]]), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_7]], sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_56]], read<i32>(%[[VALUE_n_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_57:[0-9]+]] @alloc_anti_range_57(%[[VALUE7:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_57:[0-9]+]] @test_anti_range_57(%[[VALUE_n_8:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), read<i32>(%[[VALUE_n_8]])), le<i32>(read<i32>(%[[VALUE_n_8]]), const<i32>(12)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_8]], sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_57]], read<i32>(%[[VALUE_n_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_58:[0-9]+]] @alloc_anti_range_58(%[[VALUE8:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_58:[0-9]+]] @test_anti_range_58(%[[VALUE_n_9:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), read<i32>(%[[VALUE_n_9]])), le<i32>(read<i32>(%[[VALUE_n_9]]), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1))))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_9]], sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_58]], read<i32>(%[[VALUE_n_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_61:[0-9]+]] @alloc_anti_range_61(%[[VALUE9:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_61:[0-9]+]] @test_anti_range_61(%[[VALUE_n_10:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(1), read<i32>(%[[VALUE_n_10]])), le<i32>(read<i32>(%[[VALUE_n_10]]), const<i32>(2147483647)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_10]], sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_61]], read<i32>(%[[VALUE_n_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_62:[0-9]+]] @alloc_anti_range_62(%[[VALUE10:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_62:[0-9]+]] @test_anti_range_62(%[[VALUE_n_11:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(const<i32>(0), read<i32>(%[[VALUE_n_11]])), le<i32>(read<i32>(%[[VALUE_n_11]]), const<i32>(2147483647)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_11]], sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_alloc_anti_range_62]], read<i32>(%[[VALUE_n_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_64:[0-9]+]] @alloc_anti_range_64(%[[VALUE11:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_64:[0-9]+]] @test_anti_range_64(%[[VALUE_n_12:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_12]])), le<i64>(read<i64>(%[[VALUE_n_12]]), widen<i64, reason=usual_arith>(const<i32>(12))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_12]], sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_64]], read<i64>(%[[VALUE_n_12]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_65:[0-9]+]] @alloc_anti_range_65(%[[VALUE12:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_65:[0-9]+]] @test_anti_range_65(%[[VALUE_n_13:[0-9]+]] n: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_13]])), le<i64>(read<i64>(%[[VALUE_n_13]]), widen<i64, reason=usual_arith>(const<i32>(12))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_13]], sub<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_alloc_anti_range_65]], read<i64>(%[[VALUE_n_13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_67:[0-9]+]] @alloc_anti_range_67(%[[VALUE13:[0-9]+]] <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_67:[0-9]+]] @test_anti_range_67(%[[VALUE_n_14:[0-9]+]] n: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)), read<u32>(%[[VALUE_n_14]])), le<u32>(read<u32>(%[[VALUE_n_14]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(12))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_n_14]], reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_alloc_anti_range_67]], read<u32>(%[[VALUE_n_14]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_68:[0-9]+]] @alloc_anti_range_68(%[[VALUE14:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_68:[0-9]+]] @test_anti_range_68(%[[VALUE_n_15:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%[[VALUE_n_15]])), le<u64>(read<u64>(%[[VALUE_n_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12)))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_15]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_68]], read<u64>(%[[VALUE_n_15]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_anti_range_69:[0-9]+]] @alloc_anti_range_69(%[[VALUE15:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_anti_range_69:[0-9]+]] @test_anti_range_69(%[[VALUE_n_16:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%[[VALUE_n_16]])), le<u64>(read<u64>(%[[VALUE_n_16]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12)))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_16]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sub<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc_anti_range_69]], read<u64>(%[[VALUE_n_16]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
