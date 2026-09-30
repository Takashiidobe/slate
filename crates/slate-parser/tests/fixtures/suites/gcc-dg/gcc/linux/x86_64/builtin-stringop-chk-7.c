/* Verify that -Wstringop-overflow doesn't cause false positives for
   anti-ranges.  Note that not all of the statements below result in
   the memset argument being represented as an anti-range.

   { dg-do compile }
   { dg-options "-O2 -Wstringop-overflow" } */

#define SCHAR_MAX __SCHAR_MAX__
#define UCHAR_MAX (SCHAR_MAX * 2 + 1)

#define SHRT_MAX  __SHRT_MAX__
#define USHRT_MAX (SHRT_MAX * 2U + 1)

#define INT_MAX   __INT_MAX__
#define UINT_MAX  (INT_MAX * 2U + 1)

#define LONG_MAX __LONG_MAX__
#define ULONG_MAX (LONG_MAX * 2LU + 1)

#define PTRDIFF_MAX __PTRDIFF_MAX__
#define SIZE_MAX    __SIZE_MAX__

typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__    size_t;

#define TEST_AR_1(T, prefix)			\
  void test_ar_1_ ## prefix (void *d, T n)	\
  {						\
    if (n == prefix ## _MAX - 1)		\
      n = prefix ## _MAX - 2;			\
    __builtin_memset (d, 0, n);			\
  } typedef void dummy

#define TEST_AR_2(T, prefix)					\
  void test_ar_2_ ## prefix (void *d, T n)			\
  {								\
    if (prefix ## _MAX - 2 <= n && n <= prefix ## _MAX - 1)	\
      n = prefix ## _MAX - 3;					\
    __builtin_memset (d, 0, n);					\
  } typedef void dummy

/* Verify antirange where MIN == MAX.  */
TEST_AR_1 (signed char, SCHAR);
TEST_AR_1 (unsigned char, UCHAR);

TEST_AR_1 (short, SHRT);
TEST_AR_1 (unsigned short, USHRT);

TEST_AR_1 (int, INT);
TEST_AR_1 (unsigned, UINT);

TEST_AR_1 (long, LONG);
TEST_AR_1 (unsigned long, ULONG);

TEST_AR_1 (ptrdiff_t, PTRDIFF);
TEST_AR_1 (size_t, SIZE);

/* Verify antirange where MIN < MAX.  */
TEST_AR_2 (signed char, SCHAR);
TEST_AR_2 (unsigned char, UCHAR);

TEST_AR_2 (short, SHRT);
TEST_AR_2 (unsigned short, USHRT);

TEST_AR_2 (int, INT);
TEST_AR_2 (unsigned, UINT);

TEST_AR_2 (long, LONG);
TEST_AR_2 (unsigned long, ULONG);

TEST_AR_2 (ptrdiff_t, PTRDIFF);
TEST_AR_2 (size_t, SIZE);

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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_1_SCHAR:[0-9]+]] @test_ar_1_SCHAR(%[[VALUE_d:[0-9]+]] d: ptr<void>, %[[VALUE_n:[0-9]+]] n: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n]])), sub<i32, overflow=ub>(const<i32>(127), const<i32>(1)))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d]]), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i8>(%[[VALUE_n]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_1_UCHAR:[0-9]+]] @test_ar_1_UCHAR(%[[VALUE_d_2:[0-9]+]] d: ptr<void>, %[[VALUE_n_2:[0-9]+]] n: u8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_2]]))), sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:             write<u8>(%[[VALUE_n_2]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(2)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_2]]), const<i32>(0), widen<u64, reason=arg>(read<u8>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_1_SHRT:[0-9]+]] @test_ar_1_SHRT(%[[VALUE_d_3:[0-9]+]] d: ptr<void>, %[[VALUE_n_3:[0-9]+]] n: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_3]])), sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1)))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_3]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_3]]), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i16>(%[[VALUE_n_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_1_USHRT:[0-9]+]] @test_ar_1_USHRT(%[[VALUE_d_4:[0-9]+]] d: ptr<void>, %[[VALUE_n_4:[0-9]+]] n: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_4]])))), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             write<u16>(%[[VALUE_n_4]], truncate<u16, reason=assign, fits=unknown>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_4]]), const<i32>(0), widen<u64, reason=arg>(read<u16>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_1_INT:[0-9]+]] @test_ar_1_INT(%[[VALUE_d_5:[0-9]+]] d: ptr<void>, %[[VALUE_n_5:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_n_5]]), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_5]], sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(2)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_5]]), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_5]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_1_UINT:[0-9]+]] @test_ar_1_UINT(%[[VALUE_d_6:[0-9]+]] d: ptr<void>, %[[VALUE_n_6:[0-9]+]] n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE_n_6]]), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_n_6]], sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_6]]), const<i32>(0), widen<u64, reason=arg>(read<u32>(%[[VALUE_n_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_1_LONG:[0-9]+]] @test_ar_1_LONG(%[[VALUE_d_7:[0-9]+]] d: ptr<void>, %[[VALUE_n_7:[0-9]+]] n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i64>(read<i64>(%[[VALUE_n_7]]), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_7]], sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_7]]), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_n_7]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_1_ULONG:[0-9]+]] @test_ar_1_ULONG(%[[VALUE_d_8:[0-9]+]] d: ptr<void>, %[[VALUE_n_8:[0-9]+]] n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE_n_8]]), sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_8]], sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_8]]), const<i32>(0), read<u64>(%[[VALUE_n_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_1_PTRDIFF:[0-9]+]] @test_ar_1_PTRDIFF(%[[VALUE_d_9:[0-9]+]] d: ptr<void>, %[[VALUE_n_9:[0-9]+]] n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i64>(read<i64>(%[[VALUE_n_9]]), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_9]], sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_9]]), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_n_9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_1_SIZE:[0-9]+]] @test_ar_1_SIZE(%[[VALUE_d_10:[0-9]+]] d: ptr<void>, %[[VALUE_n_10:[0-9]+]] n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE_n_10]]), sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_10]], sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_10]]), const<i32>(0), read<u64>(%[[VALUE_n_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_2_SCHAR:[0-9]+]] @test_ar_2_SCHAR(%[[VALUE_d_11:[0-9]+]] d: ptr<void>, %[[VALUE_n_11:[0-9]+]] n: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_11]]))), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_11]])), sub<i32, overflow=ub>(const<i32>(127), const<i32>(1))))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_n_11]], truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_11]]), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i8>(%[[VALUE_n_11]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_2_UCHAR:[0-9]+]] @test_ar_2_UCHAR(%[[VALUE_d_12:[0-9]+]] d: ptr<void>, %[[VALUE_n_12:[0-9]+]] n: u8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(2)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_12]])))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_12]]))), sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1))))
// DEFAULT-NEXT:             write<u8>(%[[VALUE_n_12]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_12]]), const<i32>(0), widen<u64, reason=arg>(read<u8>(%[[VALUE_n_12]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_2_SHRT:[0-9]+]] @test_ar_2_SHRT(%[[VALUE_d_13:[0-9]+]] d: ptr<void>, %[[VALUE_n_13:[0-9]+]] n: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_13]]))), le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_13]])), sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1))))
// DEFAULT-NEXT:             write<i16>(%[[VALUE_n_13]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_13]]), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i16>(%[[VALUE_n_13]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_2_USHRT:[0-9]+]] @test_ar_2_USHRT(%[[VALUE_d_14:[0-9]+]] d: ptr<void>, %[[VALUE_n_14:[0-9]+]] n: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_14]]))))), le<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_14]])))), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u16>(%[[VALUE_n_14]], truncate<u16, reason=assign, fits=unknown>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_14]]), const<i32>(0), widen<u64, reason=arg>(read<u16>(%[[VALUE_n_14]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_2_INT:[0-9]+]] @test_ar_2_INT(%[[VALUE_d_15:[0-9]+]] d: ptr<void>, %[[VALUE_n_15:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(2)), read<i32>(%[[VALUE_n_15]])), le<i32>(read<i32>(%[[VALUE_n_15]]), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1))))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_15]], sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(3)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_15]]), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_15]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_2_UINT:[0-9]+]] @test_ar_2_UINT(%[[VALUE_d_16:[0-9]+]] d: ptr<void>, %[[VALUE_n_16:[0-9]+]] n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), read<u32>(%[[VALUE_n_16]])), le<u32>(read<u32>(%[[VALUE_n_16]]), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_n_16]], sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_16]]), const<i32>(0), widen<u64, reason=arg>(read<u32>(%[[VALUE_n_16]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_2_LONG:[0-9]+]] @test_ar_2_LONG(%[[VALUE_d_17:[0-9]+]] d: ptr<void>, %[[VALUE_n_17:[0-9]+]] n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_17]])), le<i64>(read<i64>(%[[VALUE_n_17]]), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_17]], sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_17]]), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_n_17]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_2_ULONG:[0-9]+]] @test_ar_2_ULONG(%[[VALUE_d_18:[0-9]+]] d: ptr<void>, %[[VALUE_n_18:[0-9]+]] n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), read<u64>(%[[VALUE_n_18]])), le<u64>(read<u64>(%[[VALUE_n_18]]), sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_18]], sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_18]]), const<i32>(0), read<u64>(%[[VALUE_n_18]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_2_PTRDIFF:[0-9]+]] @test_ar_2_PTRDIFF(%[[VALUE_d_19:[0-9]+]] d: ptr<void>, %[[VALUE_n_19:[0-9]+]] n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%[[VALUE_n_19]])), le<i64>(read<i64>(%[[VALUE_n_19]]), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_n_19]], sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_19]]), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_n_19]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ar_2_SIZE:[0-9]+]] @test_ar_2_SIZE(%[[VALUE_d_20:[0-9]+]] d: ptr<void>, %[[VALUE_n_20:[0-9]+]] n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), read<u64>(%[[VALUE_n_20]])), le<u64>(read<u64>(%[[VALUE_n_20]]), sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_n_20]], sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_d_20]]), const<i32>(0), read<u64>(%[[VALUE_n_20]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
