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
// DEFAULT-NEXT:     type @type18 dummy = void;
// DEFAULT-NEXT:     type @type19 dummy = void;
// DEFAULT-NEXT:     type @type20 dummy = void;
// DEFAULT-NEXT:     type @type21 dummy = void;
// DEFAULT-NEXT:     fn %66 @__builtin_memset(%63 <unnamed>: ptr<void>, %64 <unnamed>: i32, %65 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @test_ar_1_SCHAR(%3 d: ptr<void>, %4 n: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(widen<i32, reason=promotion>(read<i8>(%4)), sub<i32, overflow=ub>(const<i32>(127), const<i32>(1)))
// DEFAULT-NEXT:             write<i8>(%4, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%3), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i8>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_ar_1_UCHAR(%7 d: ptr<void>, %8 n: u8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8))), sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:             write<u8>(%8, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(2)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%7), const<i32>(0), widen<u64, reason=arg>(read<u8>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test_ar_1_SHRT(%10 d: ptr<void>, %11 n: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(widen<i32, reason=promotion>(read<i16>(%11)), sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1)))
// DEFAULT-NEXT:             write<i16>(%11, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%10), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i16>(%11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test_ar_1_USHRT(%13 d: ptr<void>, %14 n: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%14)))), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             write<u16>(%14, truncate<u16, reason=assign, fits=unknown>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%13), const<i32>(0), widen<u64, reason=arg>(read<u16>(%14)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test_ar_1_INT(%16 d: ptr<void>, %17 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%17), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1)))
// DEFAULT-NEXT:             write<i32>(%17, sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(2)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%16), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%17))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_ar_1_UINT(%19 d: ptr<void>, %20 n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%20), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             write<u32>(%20, sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%19), const<i32>(0), widen<u64, reason=arg>(read<u32>(%20)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test_ar_1_LONG(%22 d: ptr<void>, %23 n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i64>(read<i64>(%23), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%23, sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%22), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%23)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @test_ar_1_ULONG(%25 d: ptr<void>, %26 n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%26), sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u64>(%26, sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%25), const<i32>(0), read<u64>(%26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @test_ar_1_PTRDIFF(%28 d: ptr<void>, %29 n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i64>(read<i64>(%29), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%29, sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%28), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%29)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @test_ar_1_SIZE(%31 d: ptr<void>, %32 n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%32), sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u64>(%32, sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%31), const<i32>(0), read<u64>(%32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @test_ar_2_SCHAR(%34 d: ptr<void>, %35 n: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(2)), widen<i32, reason=promotion>(read<i8>(%35))), le<i32>(widen<i32, reason=promotion>(read<i8>(%35)), sub<i32, overflow=ub>(const<i32>(127), const<i32>(1))))
// DEFAULT-NEXT:             write<i8>(%35, truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%34), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i8>(%35))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @test_ar_2_UCHAR(%37 d: ptr<void>, %38 n: u8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(2)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%38)))), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%38))), sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(1))))
// DEFAULT-NEXT:             write<u8>(%38, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%37), const<i32>(0), widen<u64, reason=arg>(read<u8>(%38)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @test_ar_2_SHRT(%40 d: ptr<void>, %41 n: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), widen<i32, reason=promotion>(read<i16>(%41))), le<i32>(widen<i32, reason=promotion>(read<i16>(%41)), sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1))))
// DEFAULT-NEXT:             write<i16>(%41, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%40), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i16>(%41))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @test_ar_2_USHRT(%43 d: ptr<void>, %44 n: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%44))))), le<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%44)))), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u16>(%44, truncate<u16, reason=assign, fits=unknown>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%43), const<i32>(0), widen<u64, reason=arg>(read<u16>(%44)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @test_ar_2_INT(%46 d: ptr<void>, %47 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(2)), read<i32>(%47)), le<i32>(read<i32>(%47), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1))))
// DEFAULT-NEXT:             write<i32>(%47, sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(3)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%46), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%47))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @test_ar_2_UINT(%49 d: ptr<void>, %50 n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), read<u32>(%50)), le<u32>(read<u32>(%50), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%50, sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%49), const<i32>(0), widen<u64, reason=arg>(read<u32>(%50)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @test_ar_2_LONG(%52 d: ptr<void>, %53 n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%53)), le<i64>(read<i64>(%53), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%53, sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%52), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%53)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @test_ar_2_ULONG(%55 d: ptr<void>, %56 n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), read<u64>(%56)), le<u64>(read<u64>(%56), sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%56, sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%55), const<i32>(0), read<u64>(%56));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @test_ar_2_PTRDIFF(%58 d: ptr<void>, %59 n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i64>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))), read<i64>(%59)), le<i64>(read<i64>(%59), sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i64>(%59, sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%58), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%59)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @test_ar_2_SIZE(%61 d: ptr<void>, %62 n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), read<u64>(%62)), le<u64>(read<u64>(%62), sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<u64>(%62, sub<u64, overflow=wrap>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%66, read<ptr<void>>(%61), const<i32>(0), read<u64>(%62));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
