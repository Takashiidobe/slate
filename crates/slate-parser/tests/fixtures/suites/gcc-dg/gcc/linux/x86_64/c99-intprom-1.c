/* Test for integer promotion rules: extended to long long by C99.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

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
  /* Same type.  */
  CHECK(int, int, int);
  CHECK(unsigned int, unsigned int, unsigned int);
  CHECK(long, long, long);
  CHECK(unsigned long, unsigned long, unsigned long);
  CHECK(long long, long long, long long);
  CHECK(unsigned long long, unsigned long long, unsigned long long);
  /* Both signed.  */
  CHECK(int, long, long);
  CHECK(int, long long, long long);
  CHECK(long, int, long);
  CHECK(long, long long, long long);
  CHECK(long long, int, long long);
  CHECK(long long, long, long long);
  /* Both unsigned.  */
  CHECK(unsigned int, unsigned long, unsigned long);
  CHECK(unsigned int, unsigned long long, unsigned long long);
  CHECK(unsigned long, unsigned int, unsigned long);
  CHECK(unsigned long, unsigned long long, unsigned long long);
  CHECK(unsigned long long, unsigned int, unsigned long long);
  CHECK(unsigned long long, unsigned long, unsigned long long);
  /* Unsigned of greater or equal rank.  */
  CHECK(int, unsigned int, unsigned int);
  CHECK(int, unsigned long, unsigned long);
  CHECK(int, unsigned long long, unsigned long long);
  CHECK(unsigned int, int, unsigned int);
  CHECK(long, unsigned long, unsigned long);
  CHECK(long, unsigned long long, unsigned long long);
  CHECK(unsigned long, int, unsigned long);
  CHECK(unsigned long, long, unsigned long);
  CHECK(long long, unsigned long long, unsigned long long);
  CHECK(unsigned long long, int, unsigned long long);
  CHECK(unsigned long long, long, unsigned long long);
  CHECK(unsigned long long, long long, unsigned long long);
  /* Signed of greater rank.  */
#if LONG_MAX >= UINT_MAX
  CHECK(unsigned int, long, long);
  CHECK(long, unsigned int, long);
#else
  CHECK(unsigned int, long, unsigned long);
  CHECK(long, unsigned int, unsigned long);
#endif
#if LLONG_MAX >= UINT_MAX
  CHECK(unsigned int, long long, long long);
  CHECK(long long, unsigned int, long long);
#else
  CHECK(unsigned int, long long, unsigned long long);
  CHECK(long long, unsigned int, unsigned long long);
#endif
#if LLONG_MAX >= ULONG_MAX
  CHECK(unsigned long, long long, long long);
  CHECK(long long, unsigned long, long long);
#else
  CHECK(unsigned long, long long, unsigned long long);
  CHECK(long long, unsigned long, unsigned long long);
#endif
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:         do %145
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %1 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %2 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %3 c: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:                 let %4 d: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:                 write<ptr<i32>>(%3, read<ptr<i32>>(%4));
// DEFAULT-NEXT:                 write<ptr<i32>>(%4, read<ptr<i32>>(%3));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %146
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %5 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %6 b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %7 c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %8 d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%7, read<ptr<u32>>(%8));
// DEFAULT-NEXT:                 write<ptr<u32>>(%8, read<ptr<u32>>(%7));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %147
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %9 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %10 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %11 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %12 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%11, read<ptr<i64>>(%12));
// DEFAULT-NEXT:                 write<ptr<i64>>(%12, read<ptr<i64>>(%11));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %148
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %13 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %14 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %15 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %16 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%15, read<ptr<u64>>(%16));
// DEFAULT-NEXT:                 write<ptr<u64>>(%16, read<ptr<u64>>(%15));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %149
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %17 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %18 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %19 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %20 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%19, read<ptr<i64>>(%20));
// DEFAULT-NEXT:                 write<ptr<i64>>(%20, read<ptr<i64>>(%19));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %150
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %22 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %23 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %24 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%23, read<ptr<u64>>(%24));
// DEFAULT-NEXT:                 write<ptr<u64>>(%24, read<ptr<u64>>(%23));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %151
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %25 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %26 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %27 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %28 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%27, read<ptr<i64>>(%28));
// DEFAULT-NEXT:                 write<ptr<i64>>(%28, read<ptr<i64>>(%27));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %152
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %29 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %30 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %31 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %32 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%31, read<ptr<i64>>(%32));
// DEFAULT-NEXT:                 write<ptr<i64>>(%32, read<ptr<i64>>(%31));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %153
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %33 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %34 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %35 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %36 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%35, read<ptr<i64>>(%36));
// DEFAULT-NEXT:                 write<ptr<i64>>(%36, read<ptr<i64>>(%35));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %154
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %37 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %38 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %39 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %40 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%39, read<ptr<i64>>(%40));
// DEFAULT-NEXT:                 write<ptr<i64>>(%40, read<ptr<i64>>(%39));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %155
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %41 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %42 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %43 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %44 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%43, read<ptr<i64>>(%44));
// DEFAULT-NEXT:                 write<ptr<i64>>(%44, read<ptr<i64>>(%43));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %156
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %45 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %46 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %47 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %48 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%47, read<ptr<i64>>(%48));
// DEFAULT-NEXT:                 write<ptr<i64>>(%48, read<ptr<i64>>(%47));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %157
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %49 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %50 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %51 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %52 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%51, read<ptr<u64>>(%52));
// DEFAULT-NEXT:                 write<ptr<u64>>(%52, read<ptr<u64>>(%51));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %158
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %53 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %54 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %55 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %56 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%55, read<ptr<u64>>(%56));
// DEFAULT-NEXT:                 write<ptr<u64>>(%56, read<ptr<u64>>(%55));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %159
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %57 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %58 b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %59 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %60 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%59, read<ptr<u64>>(%60));
// DEFAULT-NEXT:                 write<ptr<u64>>(%60, read<ptr<u64>>(%59));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %160
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %61 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %62 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %63 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %64 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%63, read<ptr<u64>>(%64));
// DEFAULT-NEXT:                 write<ptr<u64>>(%64, read<ptr<u64>>(%63));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %161
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %65 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %66 b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %67 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %68 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%67, read<ptr<u64>>(%68));
// DEFAULT-NEXT:                 write<ptr<u64>>(%68, read<ptr<u64>>(%67));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %162
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %69 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %70 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %71 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %72 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%71, read<ptr<u64>>(%72));
// DEFAULT-NEXT:                 write<ptr<u64>>(%72, read<ptr<u64>>(%71));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %163
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %73 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %74 b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %75 c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %76 d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%75, read<ptr<u32>>(%76));
// DEFAULT-NEXT:                 write<ptr<u32>>(%76, read<ptr<u32>>(%75));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %164
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %77 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %78 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %79 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %80 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%79, read<ptr<u64>>(%80));
// DEFAULT-NEXT:                 write<ptr<u64>>(%80, read<ptr<u64>>(%79));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %165
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %81 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %82 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %83 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %84 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%83, read<ptr<u64>>(%84));
// DEFAULT-NEXT:                 write<ptr<u64>>(%84, read<ptr<u64>>(%83));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %166
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %85 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %86 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %87 c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %88 d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%87, read<ptr<u32>>(%88));
// DEFAULT-NEXT:                 write<ptr<u32>>(%88, read<ptr<u32>>(%87));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %167
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %89 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %90 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %91 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %92 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%91, read<ptr<u64>>(%92));
// DEFAULT-NEXT:                 write<ptr<u64>>(%92, read<ptr<u64>>(%91));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %168
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %93 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %94 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %95 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %96 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%95, read<ptr<u64>>(%96));
// DEFAULT-NEXT:                 write<ptr<u64>>(%96, read<ptr<u64>>(%95));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %169
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %97 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %98 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %99 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %100 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%99, read<ptr<u64>>(%100));
// DEFAULT-NEXT:                 write<ptr<u64>>(%100, read<ptr<u64>>(%99));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %170
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %101 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %102 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %103 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %104 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%103, read<ptr<u64>>(%104));
// DEFAULT-NEXT:                 write<ptr<u64>>(%104, read<ptr<u64>>(%103));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %171
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %105 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %106 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %107 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %108 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%107, read<ptr<u64>>(%108));
// DEFAULT-NEXT:                 write<ptr<u64>>(%108, read<ptr<u64>>(%107));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %172
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %109 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %110 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %111 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %112 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%111, read<ptr<u64>>(%112));
// DEFAULT-NEXT:                 write<ptr<u64>>(%112, read<ptr<u64>>(%111));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %173
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %113 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %114 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %115 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %116 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%115, read<ptr<u64>>(%116));
// DEFAULT-NEXT:                 write<ptr<u64>>(%116, read<ptr<u64>>(%115));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %174
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %117 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %118 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %119 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %120 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%119, read<ptr<u64>>(%120));
// DEFAULT-NEXT:                 write<ptr<u64>>(%120, read<ptr<u64>>(%119));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %175
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %121 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %122 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %123 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %124 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%123, read<ptr<i64>>(%124));
// DEFAULT-NEXT:                 write<ptr<i64>>(%124, read<ptr<i64>>(%123));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %176
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %125 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %126 b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %127 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %128 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%127, read<ptr<i64>>(%128));
// DEFAULT-NEXT:                 write<ptr<i64>>(%128, read<ptr<i64>>(%127));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %177
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %129 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %130 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %131 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %132 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%131, read<ptr<i64>>(%132));
// DEFAULT-NEXT:                 write<ptr<i64>>(%132, read<ptr<i64>>(%131));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %178
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %133 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %134 b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %135 c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %136 d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%135, read<ptr<i64>>(%136));
// DEFAULT-NEXT:                 write<ptr<i64>>(%136, read<ptr<i64>>(%135));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %179
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %137 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %138 b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %139 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %140 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%139, read<ptr<u64>>(%140));
// DEFAULT-NEXT:                 write<ptr<u64>>(%140, read<ptr<u64>>(%139));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %180
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %141 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %142 b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %143 c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %144 d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%143, read<ptr<u64>>(%144));
// DEFAULT-NEXT:                 write<ptr<u64>>(%144, read<ptr<u64>>(%143));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
