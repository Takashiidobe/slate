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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_c:[0-9]+]] c: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:                 let %[[VALUE_d:[0-9]+]] d: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_c]], read<ptr<i32>>(%[[VALUE_d]]));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_d]], read<ptr<i32>>(%[[VALUE_c]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_2:[0-9]+]] a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_2:[0-9]+]] b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_2:[0-9]+]] c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_2:[0-9]+]] d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_c_2]], read<ptr<u32>>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_d_2]], read<ptr<u32>>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_3:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_3:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_3:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_3:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_3]], read<ptr<i64>>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_3]], read<ptr<i64>>(%[[VALUE_c_3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_4:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_4:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_4:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_4:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_4]], read<ptr<u64>>(%[[VALUE_d_4]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_4]], read<ptr<u64>>(%[[VALUE_c_4]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_5:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_5:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_5:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_5:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_5]], read<ptr<i64>>(%[[VALUE_d_5]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_5]], read<ptr<i64>>(%[[VALUE_c_5]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_6:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_6:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_6:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_6:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_6]], read<ptr<u64>>(%[[VALUE_d_6]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_6]], read<ptr<u64>>(%[[VALUE_c_6]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_7:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_b_7:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_7:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_7:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_7]], read<ptr<i64>>(%[[VALUE_d_7]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_7]], read<ptr<i64>>(%[[VALUE_c_7]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_8:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_b_8:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_8:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_8:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_8]], read<ptr<i64>>(%[[VALUE_d_8]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_8]], read<ptr<i64>>(%[[VALUE_c_8]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_9:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_9:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_c_9:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_9:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_9]], read<ptr<i64>>(%[[VALUE_d_9]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_9]], read<ptr<i64>>(%[[VALUE_c_9]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_10:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_10:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_10:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_10:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_10]], read<ptr<i64>>(%[[VALUE_d_10]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_10]], read<ptr<i64>>(%[[VALUE_c_10]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_11:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_11:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_c_11:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_11:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_11]], read<ptr<i64>>(%[[VALUE_d_11]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_11]], read<ptr<i64>>(%[[VALUE_c_11]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_12:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_12:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_12:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_12:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_12]], read<ptr<i64>>(%[[VALUE_d_12]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_12]], read<ptr<i64>>(%[[VALUE_c_12]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_13:[0-9]+]] a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_13:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_13:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_13:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_13]], read<ptr<u64>>(%[[VALUE_d_13]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_13]], read<ptr<u64>>(%[[VALUE_c_13]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_14:[0-9]+]] a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_14:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_14:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_14:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_14]], read<ptr<u64>>(%[[VALUE_d_14]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_14]], read<ptr<u64>>(%[[VALUE_c_14]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_15:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_15:[0-9]+]] b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_15:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_15:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_15]], read<ptr<u64>>(%[[VALUE_d_15]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_15]], read<ptr<u64>>(%[[VALUE_c_15]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_16:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_16:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_16:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_16:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_16]], read<ptr<u64>>(%[[VALUE_d_16]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_16]], read<ptr<u64>>(%[[VALUE_c_16]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_17:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_17:[0-9]+]] b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_17:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_17:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_17]], read<ptr<u64>>(%[[VALUE_d_17]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_17]], read<ptr<u64>>(%[[VALUE_c_17]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_18:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_18:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_18:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_18:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_18]], read<ptr<u64>>(%[[VALUE_d_18]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_18]], read<ptr<u64>>(%[[VALUE_c_18]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_19:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_b_19:[0-9]+]] b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_19:[0-9]+]] c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_19:[0-9]+]] d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_c_19]], read<ptr<u32>>(%[[VALUE_d_19]]));
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_d_19]], read<ptr<u32>>(%[[VALUE_c_19]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_20:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_b_20:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_20:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_20:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_20]], read<ptr<u64>>(%[[VALUE_d_20]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_20]], read<ptr<u64>>(%[[VALUE_c_20]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_21:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_b_21:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_21:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_21:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_21]], read<ptr<u64>>(%[[VALUE_d_21]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_21]], read<ptr<u64>>(%[[VALUE_c_21]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_22:[0-9]+]] a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_22:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_c_22:[0-9]+]] c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_22:[0-9]+]] d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_c_22]], read<ptr<u32>>(%[[VALUE_d_22]]));
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_d_22]], read<ptr<u32>>(%[[VALUE_c_22]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_23:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_23:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_23:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_23:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_23]], read<ptr<u64>>(%[[VALUE_d_23]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_23]], read<ptr<u64>>(%[[VALUE_c_23]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_24:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_24:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_24:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_24:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_24]], read<ptr<u64>>(%[[VALUE_d_24]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_24]], read<ptr<u64>>(%[[VALUE_c_24]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_25:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_25:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_c_25:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_25:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_25]], read<ptr<u64>>(%[[VALUE_d_25]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_25]], read<ptr<u64>>(%[[VALUE_c_25]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_26:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_26:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_26:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_26:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_26]], read<ptr<u64>>(%[[VALUE_d_26]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_26]], read<ptr<u64>>(%[[VALUE_c_26]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_27:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_27:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_27:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_27:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_27]], read<ptr<u64>>(%[[VALUE_d_27]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_27]], read<ptr<u64>>(%[[VALUE_c_27]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_28:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_28:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_c_28:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_28:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_28]], read<ptr<u64>>(%[[VALUE_d_28]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_28]], read<ptr<u64>>(%[[VALUE_c_28]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_29:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_29:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_29:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_29:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_29]], read<ptr<u64>>(%[[VALUE_d_29]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_29]], read<ptr<u64>>(%[[VALUE_c_29]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_30:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_30:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_30:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_30:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_30]], read<ptr<u64>>(%[[VALUE_d_30]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_30]], read<ptr<u64>>(%[[VALUE_c_30]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_31:[0-9]+]] a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_31:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_31:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_31:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_31]], read<ptr<i64>>(%[[VALUE_d_31]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_31]], read<ptr<i64>>(%[[VALUE_c_31]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_32:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_32:[0-9]+]] b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_32:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_32:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_32]], read<ptr<i64>>(%[[VALUE_d_32]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_32]], read<ptr<i64>>(%[[VALUE_c_32]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_33:[0-9]+]] a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_33:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_33:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_33:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_33]], read<ptr<i64>>(%[[VALUE_d_33]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_33]], read<ptr<i64>>(%[[VALUE_c_33]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_34:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_34:[0-9]+]] b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_34:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_34:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_34]], read<ptr<i64>>(%[[VALUE_d_34]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_34]], read<ptr<i64>>(%[[VALUE_c_34]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_35:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_35:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_35:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_35:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_35]], read<ptr<u64>>(%[[VALUE_d_35]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_35]], read<ptr<u64>>(%[[VALUE_c_35]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_36:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_36:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_36:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_36:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_36]], read<ptr<u64>>(%[[VALUE_d_36]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_36]], read<ptr<u64>>(%[[VALUE_c_36]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
