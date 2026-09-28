/* Test for integer promotion rules: C90 subset of types.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors" } */

#include <limits.h>

#define CHECK(T1, T2, TC)			\
  do {						\
    T1 a = 0;					\
    T2 b = 0;					\
    TC *c = 0;					\
    __typeof__(a+b) *d = 0;			\
    c = d;					\
    d = c;					\
  } while (0)

void
f (void)
{
  /* One type is unsigned long.  */
  CHECK(unsigned long, unsigned long, unsigned long);
  CHECK(unsigned int, unsigned long, unsigned long);
  CHECK(unsigned long, unsigned int, unsigned long);
  CHECK(int, unsigned long, unsigned long);
  CHECK(long, unsigned long, unsigned long);
  CHECK(unsigned long, int, unsigned long);
  CHECK(unsigned long, long, unsigned long);
  /* long and unsigned int.  */
#if LONG_MAX >= UINT_MAX
  CHECK(unsigned int, long, long);
  CHECK(long, unsigned int, long);
#else
  CHECK(unsigned int, long, unsigned long);
  CHECK(long, unsigned int, unsigned long);
#endif
  /* One type is long.  */
  CHECK(long, long, long);
  CHECK(int, long, long);
  CHECK(long, int, long);
  /* One type is unsigned int.  */
  CHECK(unsigned int, unsigned int, unsigned int);
  CHECK(int, unsigned int, unsigned int);
  CHECK(unsigned int, int, unsigned int);
  /* Otherwise int.  */
  CHECK(int, int, int);
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1990
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
// DEFAULT-NEXT:     fn %0 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %65
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %1 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %2 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %3 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %4 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%3, read<ptr<u64>>(%4));
// DEFAULT-NEXT:                 write<ptr<u64>>(%4, read<ptr<u64>>(%3));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %66
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %5 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %6 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %7 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %8 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%7, read<ptr<u64>>(%8));
// DEFAULT-NEXT:                 write<ptr<u64>>(%8, read<ptr<u64>>(%7));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %67
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %9 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %10 b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %11 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %12 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%11, read<ptr<u64>>(%12));
// DEFAULT-NEXT:                 write<ptr<u64>>(%12, read<ptr<u64>>(%11));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %68
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %13 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %14 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %15 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %16 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%15, read<ptr<u64>>(%16));
// DEFAULT-NEXT:                 write<ptr<u64>>(%16, read<ptr<u64>>(%15));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %69
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %17 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %18 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %19 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %20 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%19, read<ptr<u64>>(%20));
// DEFAULT-NEXT:                 write<ptr<u64>>(%20, read<ptr<u64>>(%19));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %70
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %22 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %23 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %24 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%23, read<ptr<u64>>(%24));
// DEFAULT-NEXT:                 write<ptr<u64>>(%24, read<ptr<u64>>(%23));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %71
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %25 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %26 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %27 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %28 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%27, read<ptr<u64>>(%28));
// DEFAULT-NEXT:                 write<ptr<u64>>(%28, read<ptr<u64>>(%27));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %72
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %29 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %30 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %31 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %32 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%31, read<ptr<i64>>(%32));
// DEFAULT-NEXT:                 write<ptr<i64>>(%32, read<ptr<i64>>(%31));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %73
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %33 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %34 b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %35 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %36 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%35, read<ptr<i64>>(%36));
// DEFAULT-NEXT:                 write<ptr<i64>>(%36, read<ptr<i64>>(%35));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %74
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %37 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %38 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %39 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %40 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%39, read<ptr<i64>>(%40));
// DEFAULT-NEXT:                 write<ptr<i64>>(%40, read<ptr<i64>>(%39));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %75
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %41 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %42 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %43 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %44 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%43, read<ptr<i64>>(%44));
// DEFAULT-NEXT:                 write<ptr<i64>>(%44, read<ptr<i64>>(%43));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %76
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %45 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %46 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %47 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %48 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%47, read<ptr<i64>>(%48));
// DEFAULT-NEXT:                 write<ptr<i64>>(%48, read<ptr<i64>>(%47));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %77
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %49 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %50 b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %51 c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %52 d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%51, read<ptr<u32>>(%52));
// DEFAULT-NEXT:                 write<ptr<u32>>(%52, read<ptr<u32>>(%51));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %78
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %53 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %54 b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %55 c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %56 d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%55, read<ptr<u32>>(%56));
// DEFAULT-NEXT:                 write<ptr<u32>>(%56, read<ptr<u32>>(%55));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %79
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %57 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %58 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %59 c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %60 d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%59, read<ptr<u32>>(%60));
// DEFAULT-NEXT:                 write<ptr<u32>>(%60, read<ptr<u32>>(%59));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %80
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %61 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %62 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %63 c: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:                 let %64 d: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:                 write<ptr<i32>>(%63, read<ptr<i32>>(%64));
// DEFAULT-NEXT:                 write<ptr<i32>>(%64, read<ptr<i32>>(%63));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
