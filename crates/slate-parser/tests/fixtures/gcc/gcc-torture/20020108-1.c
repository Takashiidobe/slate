/* This file tests shifts in various integral modes.  */

#include <limits.h>

#define CAT(A, B) A##B

#define REPEAT_8                                                               \
  REPEAT_FN(0)                                                                 \
  REPEAT_FN(1)                                                                 \
  REPEAT_FN(2)                                                                 \
  REPEAT_FN(3)                                                                 \
  REPEAT_FN(4)                                                                 \
  REPEAT_FN(5)                                                                 \
  REPEAT_FN(6)                                                                 \
  REPEAT_FN(7)

#define REPEAT_16                                                              \
  REPEAT_8                                                                     \
  REPEAT_FN(8)                                                                 \
  REPEAT_FN(9)                                                                 \
  REPEAT_FN(10)                                                                \
  REPEAT_FN(11)                                                                \
  REPEAT_FN(12)                                                                \
  REPEAT_FN(13)                                                                \
  REPEAT_FN(14)                                                                \
  REPEAT_FN(15)

#define REPEAT_32                                                              \
  REPEAT_16                                                                    \
  REPEAT_FN(16)                                                                \
  REPEAT_FN(17)                                                                \
  REPEAT_FN(18)                                                                \
  REPEAT_FN(19)                                                                \
  REPEAT_FN(20)                                                                \
  REPEAT_FN(21)                                                                \
  REPEAT_FN(22)                                                                \
  REPEAT_FN(23)                                                                \
  REPEAT_FN(24)                                                                \
  REPEAT_FN(25)                                                                \
  REPEAT_FN(26)                                                                \
  REPEAT_FN(27)                                                                \
  REPEAT_FN(28)                                                                \
  REPEAT_FN(29)                                                                \
  REPEAT_FN(30)                                                                \
  REPEAT_FN(31)

/* Define 8-bit shifts.  */
#if CHAR_BIT == 8
typedef unsigned int u8 __attribute__((mode(QI)));
typedef signed int   s8 __attribute__((mode(QI)));

#define REPEAT_FN(COUNT)                                                       \
  u8 CAT(ashift_qi_, COUNT)(u8 n) { return n << COUNT; }
REPEAT_8
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  u8 CAT(lshiftrt_qi_, COUNT)(u8 n) { return n >> COUNT; }
REPEAT_8
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  s8 CAT(ashiftrt_qi_, COUNT)(s8 n) { return n >> COUNT; }
REPEAT_8
#undef REPEAT_FN
#endif /* CHAR_BIT == 8 */

/* Define 16-bit shifts.  */
#if CHAR_BIT == 8 || CHAR_BIT == 16
#if CHAR_BIT == 8
typedef unsigned int u16 __attribute__((mode(HI)));
typedef signed int   s16 __attribute__((mode(HI)));
#elif CHAR_BIT == 16
typedef unsigned int u16 __attribute__((mode(QI)));
typedef signed int   s16 __attribute__((mode(QI)));
#endif

#define REPEAT_FN(COUNT)                                                       \
  u16 CAT(ashift_hi_, COUNT)(u16 n) { return n << COUNT; }
REPEAT_16
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  u16 CAT(lshiftrt_hi_, COUNT)(u16 n) { return n >> COUNT; }
REPEAT_16
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  s16 CAT(ashiftrt_hi_, COUNT)(s16 n) { return n >> COUNT; }
REPEAT_16
#undef REPEAT_FN
#endif /* CHAR_BIT == 8 || CHAR_BIT == 16 */

/* Define 32-bit shifts.  */
#if CHAR_BIT == 8 || CHAR_BIT == 16 || CHAR_BIT == 32
#if CHAR_BIT == 8
typedef unsigned int u32 __attribute__((mode(SI)));
typedef signed int   s32 __attribute__((mode(SI)));
#elif CHAR_BIT == 16
typedef unsigned int u32 __attribute__((mode(HI)));
typedef signed int   s32 __attribute__((mode(HI)));
#elif CHAR_BIT == 32
typedef unsigned int u32 __attribute__((mode(QI)));
typedef signed int   s32 __attribute__((mode(QI)));
#endif

#define REPEAT_FN(COUNT)                                                       \
  u32 CAT(ashift_si_, COUNT)(u32 n) { return n << COUNT; }
REPEAT_32
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  u32 CAT(lshiftrt_si_, COUNT)(u32 n) { return n >> COUNT; }
REPEAT_32
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  s32 CAT(ashiftrt_si_, COUNT)(s32 n) { return n >> COUNT; }
REPEAT_32
#undef REPEAT_FN
#endif /* CHAR_BIT == 8 || CHAR_BIT == 16 || CHAR_BIT == 32 */

extern void abort(void);
extern void exit(int);

int main() {
  /* Test 8-bit shifts.  */
#if CHAR_BIT == 8
#define REPEAT_FN(COUNT)                                                       \
  if (CAT(ashift_qi_, COUNT)(0xff) != (u8)((u8)0xff << COUNT))                 \
    abort();
  REPEAT_8;
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  if (CAT(lshiftrt_qi_, COUNT)(0xff) != (u8)((u8)0xff >> COUNT))               \
    abort();
  REPEAT_8;
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  if (CAT(ashiftrt_qi_, COUNT)(-1) != -1)                                      \
    abort();
  REPEAT_8;
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  if (CAT(ashiftrt_qi_, COUNT)(0) != 0)                                        \
    abort();
  REPEAT_8;
#undef REPEAT_FN
#endif /* CHAR_BIT == 8 */

  /* Test 16-bit shifts.  */
#if CHAR_BIT == 8 || CHAR_BIT == 16
#define REPEAT_FN(COUNT)                                                       \
  if (CAT(ashift_hi_, COUNT)(0xffff) != (u16)((u16)0xffff << COUNT))           \
    abort();
  REPEAT_16;
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  if (CAT(lshiftrt_hi_, COUNT)(0xffff) != (u16)((u16)0xffff >> COUNT))         \
    abort();
  REPEAT_16;
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  if (CAT(ashiftrt_hi_, COUNT)(-1) != -1)                                      \
    abort();
  REPEAT_16;
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  if (CAT(ashiftrt_hi_, COUNT)(0) != 0)                                        \
    abort();
  REPEAT_16;
#undef REPEAT_FN
#endif /* CHAR_BIT == 8 || CHAR_BIT == 16 */

  /* Test 32-bit shifts.  */
#if CHAR_BIT == 8 || CHAR_BIT == 16 || CHAR_BIT == 32
#define REPEAT_FN(COUNT)                                                       \
  if (CAT(ashift_si_, COUNT)(0xffffffff) != (u32)((u32)0xffffffff << COUNT))   \
    abort();
  REPEAT_32;
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  if (CAT(lshiftrt_si_, COUNT)(0xffffffff) != (u32)((u32)0xffffffff >> COUNT)) \
    abort();
  REPEAT_32;
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  if (CAT(ashiftrt_si_, COUNT)(-1) != -1)                                      \
    abort();
  REPEAT_32;
#undef REPEAT_FN

#define REPEAT_FN(COUNT)                                                       \
  if (CAT(ashiftrt_si_, COUNT)(0) != 0)                                        \
    abort();
  REPEAT_32;
#undef REPEAT_FN
#endif /* CHAR_BIT == 8 || CHAR_BIT == 16 || CHAR_BIT == 32 */

  exit(0);
}



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
// DEFAULT-NEXT:     type @type0 u8 = u8;
// DEFAULT-NEXT:     type @type1 s8 = i8;
// DEFAULT-NEXT:     type @type2 u16 = u16;
// DEFAULT-NEXT:     type @type3 s16 = i16;
// DEFAULT-NEXT:     type @type4 u32 = u32;
// DEFAULT-NEXT:     type @type5 s32 = i32;
// DEFAULT-NEXT:     fn %2 @ashift_qi_0(%3 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%3))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @ashift_qi_1(%5 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%5))), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @ashift_qi_2(%7 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%7))), const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @ashift_qi_3(%9 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%9))), const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @ashift_qi_4(%11 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%11))), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @ashift_qi_5(%13 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%13))), const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @ashift_qi_6(%15 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%15))), const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @ashift_qi_7(%17 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%17))), const<i32>(7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @lshiftrt_qi_0(%19 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%19))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @lshiftrt_qi_1(%21 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%21))), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @lshiftrt_qi_2(%23 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%23))), const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @lshiftrt_qi_3(%25 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%25))), const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @lshiftrt_qi_4(%27 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%27))), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @lshiftrt_qi_5(%29 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%29))), const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @lshiftrt_qi_6(%31 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%31))), const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @lshiftrt_qi_7(%33 n: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%33))), const<i32>(7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @ashiftrt_qi_0(%35 n: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%35)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @ashiftrt_qi_1(%37 n: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%37)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @ashiftrt_qi_2(%39 n: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%39)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @ashiftrt_qi_3(%41 n: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%41)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @ashiftrt_qi_4(%43 n: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%43)), const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @ashiftrt_qi_5(%45 n: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%45)), const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @ashiftrt_qi_6(%47 n: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%47)), const<i32>(6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @ashiftrt_qi_7(%49 n: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%49)), const<i32>(7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @ashift_hi_0(%53 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%53))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @ashift_hi_1(%55 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%55))), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @ashift_hi_2(%57 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%57))), const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @ashift_hi_3(%59 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%59))), const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @ashift_hi_4(%61 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%61))), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @ashift_hi_5(%63 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%63))), const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @ashift_hi_6(%65 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%65))), const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @ashift_hi_7(%67 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%67))), const<i32>(7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @ashift_hi_8(%69 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%69))), const<i32>(8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %70 @ashift_hi_9(%71 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%71))), const<i32>(9))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @ashift_hi_10(%73 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%73))), const<i32>(10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @ashift_hi_11(%75 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%75))), const<i32>(11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %76 @ashift_hi_12(%77 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%77))), const<i32>(12))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %78 @ashift_hi_13(%79 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%79))), const<i32>(13))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @ashift_hi_14(%81 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%81))), const<i32>(14))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %82 @ashift_hi_15(%83 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%83))), const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %84 @lshiftrt_hi_0(%85 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%85))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %86 @lshiftrt_hi_1(%87 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%87))), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %88 @lshiftrt_hi_2(%89 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%89))), const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @lshiftrt_hi_3(%91 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%91))), const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %92 @lshiftrt_hi_4(%93 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%93))), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %94 @lshiftrt_hi_5(%95 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%95))), const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %96 @lshiftrt_hi_6(%97 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%97))), const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %98 @lshiftrt_hi_7(%99 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%99))), const<i32>(7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %100 @lshiftrt_hi_8(%101 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%101))), const<i32>(8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %102 @lshiftrt_hi_9(%103 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%103))), const<i32>(9))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %104 @lshiftrt_hi_10(%105 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%105))), const<i32>(10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @lshiftrt_hi_11(%107 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%107))), const<i32>(11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %108 @lshiftrt_hi_12(%109 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%109))), const<i32>(12))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %110 @lshiftrt_hi_13(%111 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%111))), const<i32>(13))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %112 @lshiftrt_hi_14(%113 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%113))), const<i32>(14))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %114 @lshiftrt_hi_15(%115 n: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%115))), const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %116 @ashiftrt_hi_0(%117 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%117)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %118 @ashiftrt_hi_1(%119 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%119)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %120 @ashiftrt_hi_2(%121 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%121)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %122 @ashiftrt_hi_3(%123 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%123)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %124 @ashiftrt_hi_4(%125 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%125)), const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %126 @ashiftrt_hi_5(%127 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%127)), const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %128 @ashiftrt_hi_6(%129 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%129)), const<i32>(6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %130 @ashiftrt_hi_7(%131 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%131)), const<i32>(7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %132 @ashiftrt_hi_8(%133 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%133)), const<i32>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %134 @ashiftrt_hi_9(%135 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%135)), const<i32>(9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %136 @ashiftrt_hi_10(%137 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%137)), const<i32>(10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %138 @ashiftrt_hi_11(%139 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%139)), const<i32>(11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %140 @ashiftrt_hi_12(%141 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%141)), const<i32>(12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %142 @ashiftrt_hi_13(%143 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%143)), const<i32>(13)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %144 @ashiftrt_hi_14(%145 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%145)), const<i32>(14)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %146 @ashiftrt_hi_15(%147 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%147)), const<i32>(15)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %150 @ashift_si_0(%151 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%151), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %152 @ashift_si_1(%153 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%153), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %154 @ashift_si_2(%155 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%155), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %156 @ashift_si_3(%157 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%157), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %158 @ashift_si_4(%159 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%159), const<i32>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %160 @ashift_si_5(%161 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%161), const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %162 @ashift_si_6(%163 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%163), const<i32>(6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %164 @ashift_si_7(%165 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%165), const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %166 @ashift_si_8(%167 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%167), const<i32>(8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %168 @ashift_si_9(%169 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%169), const<i32>(9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %170 @ashift_si_10(%171 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%171), const<i32>(10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %172 @ashift_si_11(%173 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%173), const<i32>(11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %174 @ashift_si_12(%175 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%175), const<i32>(12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %176 @ashift_si_13(%177 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%177), const<i32>(13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %178 @ashift_si_14(%179 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%179), const<i32>(14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %180 @ashift_si_15(%181 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%181), const<i32>(15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %182 @ashift_si_16(%183 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%183), const<i32>(16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %184 @ashift_si_17(%185 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%185), const<i32>(17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %186 @ashift_si_18(%187 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%187), const<i32>(18));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %188 @ashift_si_19(%189 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%189), const<i32>(19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %190 @ashift_si_20(%191 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%191), const<i32>(20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %192 @ashift_si_21(%193 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%193), const<i32>(21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %194 @ashift_si_22(%195 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%195), const<i32>(22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %196 @ashift_si_23(%197 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%197), const<i32>(23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %198 @ashift_si_24(%199 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%199), const<i32>(24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %200 @ashift_si_25(%201 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%201), const<i32>(25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %202 @ashift_si_26(%203 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%203), const<i32>(26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %204 @ashift_si_27(%205 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%205), const<i32>(27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %206 @ashift_si_28(%207 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%207), const<i32>(28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %208 @ashift_si_29(%209 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%209), const<i32>(29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %210 @ashift_si_30(%211 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%211), const<i32>(30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %212 @ashift_si_31(%213 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%213), const<i32>(31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %214 @lshiftrt_si_0(%215 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%215), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %216 @lshiftrt_si_1(%217 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%217), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %218 @lshiftrt_si_2(%219 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%219), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %220 @lshiftrt_si_3(%221 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%221), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %222 @lshiftrt_si_4(%223 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%223), const<i32>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %224 @lshiftrt_si_5(%225 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%225), const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %226 @lshiftrt_si_6(%227 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%227), const<i32>(6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %228 @lshiftrt_si_7(%229 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%229), const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %230 @lshiftrt_si_8(%231 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%231), const<i32>(8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %232 @lshiftrt_si_9(%233 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%233), const<i32>(9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %234 @lshiftrt_si_10(%235 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%235), const<i32>(10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %236 @lshiftrt_si_11(%237 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%237), const<i32>(11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %238 @lshiftrt_si_12(%239 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%239), const<i32>(12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %240 @lshiftrt_si_13(%241 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%241), const<i32>(13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %242 @lshiftrt_si_14(%243 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%243), const<i32>(14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %244 @lshiftrt_si_15(%245 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%245), const<i32>(15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %246 @lshiftrt_si_16(%247 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%247), const<i32>(16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %248 @lshiftrt_si_17(%249 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%249), const<i32>(17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %250 @lshiftrt_si_18(%251 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%251), const<i32>(18));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %252 @lshiftrt_si_19(%253 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%253), const<i32>(19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %254 @lshiftrt_si_20(%255 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%255), const<i32>(20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %256 @lshiftrt_si_21(%257 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%257), const<i32>(21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %258 @lshiftrt_si_22(%259 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%259), const<i32>(22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %260 @lshiftrt_si_23(%261 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%261), const<i32>(23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %262 @lshiftrt_si_24(%263 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%263), const<i32>(24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %264 @lshiftrt_si_25(%265 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%265), const<i32>(25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %266 @lshiftrt_si_26(%267 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%267), const<i32>(26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %268 @lshiftrt_si_27(%269 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%269), const<i32>(27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %270 @lshiftrt_si_28(%271 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%271), const<i32>(28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %272 @lshiftrt_si_29(%273 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%273), const<i32>(29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %274 @lshiftrt_si_30(%275 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%275), const<i32>(30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %276 @lshiftrt_si_31(%277 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%277), const<i32>(31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %278 @ashiftrt_si_0(%279 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%279), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %280 @ashiftrt_si_1(%281 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%281), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %282 @ashiftrt_si_2(%283 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%283), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %284 @ashiftrt_si_3(%285 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%285), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %286 @ashiftrt_si_4(%287 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%287), const<i32>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %288 @ashiftrt_si_5(%289 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%289), const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %290 @ashiftrt_si_6(%291 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%291), const<i32>(6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %292 @ashiftrt_si_7(%293 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%293), const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %294 @ashiftrt_si_8(%295 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%295), const<i32>(8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %296 @ashiftrt_si_9(%297 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%297), const<i32>(9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %298 @ashiftrt_si_10(%299 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%299), const<i32>(10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %300 @ashiftrt_si_11(%301 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%301), const<i32>(11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %302 @ashiftrt_si_12(%303 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%303), const<i32>(12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %304 @ashiftrt_si_13(%305 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%305), const<i32>(13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %306 @ashiftrt_si_14(%307 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%307), const<i32>(14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %308 @ashiftrt_si_15(%309 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%309), const<i32>(15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %310 @ashiftrt_si_16(%311 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%311), const<i32>(16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %312 @ashiftrt_si_17(%313 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%313), const<i32>(17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %314 @ashiftrt_si_18(%315 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%315), const<i32>(18));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %316 @ashiftrt_si_19(%317 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%317), const<i32>(19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %318 @ashiftrt_si_20(%319 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%319), const<i32>(20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %320 @ashiftrt_si_21(%321 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%321), const<i32>(21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %322 @ashiftrt_si_22(%323 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%323), const<i32>(22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %324 @ashiftrt_si_23(%325 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%325), const<i32>(23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %326 @ashiftrt_si_24(%327 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%327), const<i32>(24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %328 @ashiftrt_si_25(%329 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%329), const<i32>(25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %330 @ashiftrt_si_26(%331 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%331), const<i32>(26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %332 @ashiftrt_si_27(%333 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%333), const<i32>(27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %334 @ashiftrt_si_28(%335 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%335), const<i32>(28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %336 @ashiftrt_si_29(%337 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%337), const<i32>(29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %338 @ashiftrt_si_30(%339 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%339), const<i32>(30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %340 @ashiftrt_si_31(%341 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%341), const<i32>(31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %342 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %343 @exit(%345 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %344 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%2, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(0)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%4, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%6, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(2)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%8, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(3)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%10, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%12, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(5)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%14, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(6)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%16, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(7)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%18, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(0)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%20, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%22, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(2)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%24, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(3)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%26, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%28, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(5)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%30, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(6)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%32, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(7)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%34, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%36, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%38, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%40, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%42, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%44, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%46, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%48, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%34, truncate<i8, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%36, truncate<i8, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%38, truncate<i8, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%40, truncate<i8, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%42, truncate<i8, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%44, truncate<i8, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%46, truncate<i8, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%48, truncate<i8, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%52, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(0)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%54, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%56, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(2)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%58, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(3)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%60, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%62, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(5)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%64, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(6)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%66, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(7)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%68, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(8)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%70, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(9)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%72, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(10)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%74, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(11)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%76, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(12)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%78, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(13)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%80, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(14)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%82, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(15)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%84, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(0)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%86, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%88, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(2)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%90, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(3)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%92, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%94, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(5)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%96, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(6)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%98, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(7)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%100, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(8)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%102, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(9)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%104, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(10)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%106, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(11)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%108, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(12)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%110, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(13)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%112, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(14)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%114, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), const<i32>(15)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%116, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%118, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%120, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%122, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%124, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%126, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%128, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%130, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%132, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%134, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%136, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%138, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%140, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%142, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%144, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%146, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%116, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%118, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%120, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%122, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%124, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%126, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%128, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%130, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%132, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%134, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%136, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%138, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%140, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%142, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%144, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%146, truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%150, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%152, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%154, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%156, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%158, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%160, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%162, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%164, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%166, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%168, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%170, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%172, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%174, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%176, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%178, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(14)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%180, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%182, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(16)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%184, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(17)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%186, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(18)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%188, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(19)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%190, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(20)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%192, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(21)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%194, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(22)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%196, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(23)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%198, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(24)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%200, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(25)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%202, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(26)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%204, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(27)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%206, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(28)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%208, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(29)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%210, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(30)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%212, const<u32>(4294967295)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4294967295), const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%214, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%216, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%218, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%220, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%222, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%224, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%226, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%228, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%230, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%232, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%234, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%236, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%238, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%240, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%242, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(14)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%244, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%246, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(16)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%248, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(17)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%250, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(18)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%252, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(19)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%254, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(20)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%256, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(21)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%258, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(22)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%260, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(23)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%262, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(24)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%264, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(25)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%266, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(26)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%268, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(27)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%270, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(28)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%272, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(29)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%274, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(30)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%276, const<u32>(4294967295)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4294967295), const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%278, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%280, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%282, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%284, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%286, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%288, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%290, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%292, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%294, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%296, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%298, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%300, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%302, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%304, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%306, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%308, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%310, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%312, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%314, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%316, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%318, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%320, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%322, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%324, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%326, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%328, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%330, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%332, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%334, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%336, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%338, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%340, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%278, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%280, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%282, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%284, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%286, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%288, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%290, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%292, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%294, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%296, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%298, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%300, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%302, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%304, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%306, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%308, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%310, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%312, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%314, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%316, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%318, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%320, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%322, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%324, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%326, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%328, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%330, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%332, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%334, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%336, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%338, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%340, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%342);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%343, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
