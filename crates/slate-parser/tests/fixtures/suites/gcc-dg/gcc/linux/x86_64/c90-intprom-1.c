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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c]], read<ptr<u64>>(%[[VALUE_d]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d]], read<ptr<u64>>(%[[VALUE_c]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_2:[0-9]+]] a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_2:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_2:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_2:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_2]], read<ptr<u64>>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_2]], read<ptr<u64>>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_3:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_3:[0-9]+]] b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_3:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_3:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_3]], read<ptr<u64>>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_3]], read<ptr<u64>>(%[[VALUE_c_3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_4:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
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
// DEFAULT-NEXT:                 let %[[VALUE_b_5:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_c_5:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_5:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_5]], read<ptr<u64>>(%[[VALUE_d_5]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_5]], read<ptr<u64>>(%[[VALUE_c_5]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_6:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_6:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_c_6:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_6:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_6]], read<ptr<u64>>(%[[VALUE_d_6]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_6]], read<ptr<u64>>(%[[VALUE_c_6]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_7:[0-9]+]] a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE_b_7:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_7:[0-9]+]] c: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_7:[0-9]+]] d: ptr<u64> [storage=automatic] = null<ptr<u64>>;
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_c_7]], read<ptr<u64>>(%[[VALUE_d_7]]));
// DEFAULT-NEXT:                 write<ptr<u64>>(%[[VALUE_d_7]], read<ptr<u64>>(%[[VALUE_c_7]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_8:[0-9]+]] a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
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
// DEFAULT-NEXT:                 let %[[VALUE_b_9:[0-9]+]] b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
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
// DEFAULT-NEXT:                 let %[[VALUE_a_11:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_b_11:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_11:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_11:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_11]], read<ptr<i64>>(%[[VALUE_d_11]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_11]], read<ptr<i64>>(%[[VALUE_c_11]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_12:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_12:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_c_12:[0-9]+]] c: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_12:[0-9]+]] d: ptr<i64> [storage=automatic] = null<ptr<i64>>;
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_c_12]], read<ptr<i64>>(%[[VALUE_d_12]]));
// DEFAULT-NEXT:                 write<ptr<i64>>(%[[VALUE_d_12]], read<ptr<i64>>(%[[VALUE_c_12]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_13:[0-9]+]] a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_13:[0-9]+]] b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_13:[0-9]+]] c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_13:[0-9]+]] d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_c_13]], read<ptr<u32>>(%[[VALUE_d_13]]));
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_d_13]], read<ptr<u32>>(%[[VALUE_c_13]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_14:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_b_14:[0-9]+]] b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_c_14:[0-9]+]] c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_14:[0-9]+]] d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_c_14]], read<ptr<u32>>(%[[VALUE_d_14]]));
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_d_14]], read<ptr<u32>>(%[[VALUE_c_14]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_15:[0-9]+]] a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_b_15:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_c_15:[0-9]+]] c: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_15:[0-9]+]] d: ptr<u32> [storage=automatic] = null<ptr<u32>>;
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_c_15]], read<ptr<u32>>(%[[VALUE_d_15]]));
// DEFAULT-NEXT:                 write<ptr<u32>>(%[[VALUE_d_15]], read<ptr<u32>>(%[[VALUE_c_15]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_16:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_b_16:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_c_16:[0-9]+]] c: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:                 let %[[VALUE_d_16:[0-9]+]] d: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_c_16]], read<ptr<i32>>(%[[VALUE_d_16]]));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_d_16]], read<ptr<i32>>(%[[VALUE_c_16]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
