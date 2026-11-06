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
// DEFAULT-NEXT:     type @type[[TYPE_u8:[0-9]+]] u8 = u8;
// DEFAULT-NEXT:     type @type[[TYPE_s8:[0-9]+]] s8 = i8;
// DEFAULT-NEXT:     type @type[[TYPE_u16:[0-9]+]] u16 = u16;
// DEFAULT-NEXT:     type @type[[TYPE_s16:[0-9]+]] s16 = i16;
// DEFAULT-NEXT:     type @type[[TYPE_u32:[0-9]+]] u32 = u32;
// DEFAULT-NEXT:     type @type[[TYPE_s32:[0-9]+]] s32 = i32;
// DEFAULT-NEXT:     fn %[[VALUE_ashift_qi_0:[0-9]+]] @ashift_qi_0(%[[VALUE_n:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n]]) [c="u8"])), const<i32>(0)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_qi_1:[0-9]+]] @ashift_qi_1(%[[VALUE_n_2:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_2]]) [c="u8"])), const<i32>(1)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_qi_2:[0-9]+]] @ashift_qi_2(%[[VALUE_n_3:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_3]]) [c="u8"])), const<i32>(2)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_qi_3:[0-9]+]] @ashift_qi_3(%[[VALUE_n_4:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_4]]) [c="u8"])), const<i32>(3)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_qi_4:[0-9]+]] @ashift_qi_4(%[[VALUE_n_5:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_5]]) [c="u8"])), const<i32>(4)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_qi_5:[0-9]+]] @ashift_qi_5(%[[VALUE_n_6:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_6]]) [c="u8"])), const<i32>(5)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_qi_6:[0-9]+]] @ashift_qi_6(%[[VALUE_n_7:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_7]]) [c="u8"])), const<i32>(6)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_qi_7:[0-9]+]] @ashift_qi_7(%[[VALUE_n_8:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_8]]) [c="u8"])), const<i32>(7)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_qi_0:[0-9]+]] @lshiftrt_qi_0(%[[VALUE_n_9:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_9]]) [c="u8"])), const<i32>(0)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_qi_1:[0-9]+]] @lshiftrt_qi_1(%[[VALUE_n_10:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_10]]) [c="u8"])), const<i32>(1)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_qi_2:[0-9]+]] @lshiftrt_qi_2(%[[VALUE_n_11:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_11]]) [c="u8"])), const<i32>(2)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_qi_3:[0-9]+]] @lshiftrt_qi_3(%[[VALUE_n_12:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_12]]) [c="u8"])), const<i32>(3)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_qi_4:[0-9]+]] @lshiftrt_qi_4(%[[VALUE_n_13:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_13]]) [c="u8"])), const<i32>(4)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_qi_5:[0-9]+]] @lshiftrt_qi_5(%[[VALUE_n_14:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_14]]) [c="u8"])), const<i32>(5)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_qi_6:[0-9]+]] @lshiftrt_qi_6(%[[VALUE_n_15:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_15]]) [c="u8"])), const<i32>(6)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_qi_7:[0-9]+]] @lshiftrt_qi_7(%[[VALUE_n_16:[0-9]+]] n: u8 [c="u8"]) -> u8 [linkage=external] [fallthrough=ub_if_used] [c="u8(u8)"] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_n_16]]) [c="u8"])), const<i32>(7)))) [c="u8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_qi_0:[0-9]+]] @ashiftrt_qi_0(%[[VALUE_n_17:[0-9]+]] n: i8 [c="s8"]) -> i8 [linkage=external] [fallthrough=ub_if_used] [c="s8(s8)"] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_17]]) [c="s8"]), const<i32>(0))) [c="s8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_qi_1:[0-9]+]] @ashiftrt_qi_1(%[[VALUE_n_18:[0-9]+]] n: i8 [c="s8"]) -> i8 [linkage=external] [fallthrough=ub_if_used] [c="s8(s8)"] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_18]]) [c="s8"]), const<i32>(1))) [c="s8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_qi_2:[0-9]+]] @ashiftrt_qi_2(%[[VALUE_n_19:[0-9]+]] n: i8 [c="s8"]) -> i8 [linkage=external] [fallthrough=ub_if_used] [c="s8(s8)"] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_19]]) [c="s8"]), const<i32>(2))) [c="s8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_qi_3:[0-9]+]] @ashiftrt_qi_3(%[[VALUE_n_20:[0-9]+]] n: i8 [c="s8"]) -> i8 [linkage=external] [fallthrough=ub_if_used] [c="s8(s8)"] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_20]]) [c="s8"]), const<i32>(3))) [c="s8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_qi_4:[0-9]+]] @ashiftrt_qi_4(%[[VALUE_n_21:[0-9]+]] n: i8 [c="s8"]) -> i8 [linkage=external] [fallthrough=ub_if_used] [c="s8(s8)"] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_21]]) [c="s8"]), const<i32>(4))) [c="s8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_qi_5:[0-9]+]] @ashiftrt_qi_5(%[[VALUE_n_22:[0-9]+]] n: i8 [c="s8"]) -> i8 [linkage=external] [fallthrough=ub_if_used] [c="s8(s8)"] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_22]]) [c="s8"]), const<i32>(5))) [c="s8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_qi_6:[0-9]+]] @ashiftrt_qi_6(%[[VALUE_n_23:[0-9]+]] n: i8 [c="s8"]) -> i8 [linkage=external] [fallthrough=ub_if_used] [c="s8(s8)"] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_23]]) [c="s8"]), const<i32>(6))) [c="s8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_qi_7:[0-9]+]] @ashiftrt_qi_7(%[[VALUE_n_24:[0-9]+]] n: i8 [c="s8"]) -> i8 [linkage=external] [fallthrough=ub_if_used] [c="s8(s8)"] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_24]]) [c="s8"]), const<i32>(7))) [c="s8"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_0:[0-9]+]] @ashift_hi_0(%[[VALUE_n_25:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_25]]) [c="u16"])), const<i32>(0)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_1:[0-9]+]] @ashift_hi_1(%[[VALUE_n_26:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_26]]) [c="u16"])), const<i32>(1)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_2:[0-9]+]] @ashift_hi_2(%[[VALUE_n_27:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_27]]) [c="u16"])), const<i32>(2)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_3:[0-9]+]] @ashift_hi_3(%[[VALUE_n_28:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_28]]) [c="u16"])), const<i32>(3)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_4:[0-9]+]] @ashift_hi_4(%[[VALUE_n_29:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_29]]) [c="u16"])), const<i32>(4)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_5:[0-9]+]] @ashift_hi_5(%[[VALUE_n_30:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_30]]) [c="u16"])), const<i32>(5)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_6:[0-9]+]] @ashift_hi_6(%[[VALUE_n_31:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_31]]) [c="u16"])), const<i32>(6)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_7:[0-9]+]] @ashift_hi_7(%[[VALUE_n_32:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_32]]) [c="u16"])), const<i32>(7)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_8:[0-9]+]] @ashift_hi_8(%[[VALUE_n_33:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_33]]) [c="u16"])), const<i32>(8)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_9:[0-9]+]] @ashift_hi_9(%[[VALUE_n_34:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_34]]) [c="u16"])), const<i32>(9)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_10:[0-9]+]] @ashift_hi_10(%[[VALUE_n_35:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_35]]) [c="u16"])), const<i32>(10)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_11:[0-9]+]] @ashift_hi_11(%[[VALUE_n_36:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_36]]) [c="u16"])), const<i32>(11)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_12:[0-9]+]] @ashift_hi_12(%[[VALUE_n_37:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_37]]) [c="u16"])), const<i32>(12)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_13:[0-9]+]] @ashift_hi_13(%[[VALUE_n_38:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_38]]) [c="u16"])), const<i32>(13)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_14:[0-9]+]] @ashift_hi_14(%[[VALUE_n_39:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_39]]) [c="u16"])), const<i32>(14)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_hi_15:[0-9]+]] @ashift_hi_15(%[[VALUE_n_40:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_40]]) [c="u16"])), const<i32>(15)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_0:[0-9]+]] @lshiftrt_hi_0(%[[VALUE_n_41:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_41]]) [c="u16"])), const<i32>(0)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_1:[0-9]+]] @lshiftrt_hi_1(%[[VALUE_n_42:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_42]]) [c="u16"])), const<i32>(1)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_2:[0-9]+]] @lshiftrt_hi_2(%[[VALUE_n_43:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_43]]) [c="u16"])), const<i32>(2)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_3:[0-9]+]] @lshiftrt_hi_3(%[[VALUE_n_44:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_44]]) [c="u16"])), const<i32>(3)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_4:[0-9]+]] @lshiftrt_hi_4(%[[VALUE_n_45:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_45]]) [c="u16"])), const<i32>(4)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_5:[0-9]+]] @lshiftrt_hi_5(%[[VALUE_n_46:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_46]]) [c="u16"])), const<i32>(5)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_6:[0-9]+]] @lshiftrt_hi_6(%[[VALUE_n_47:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_47]]) [c="u16"])), const<i32>(6)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_7:[0-9]+]] @lshiftrt_hi_7(%[[VALUE_n_48:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_48]]) [c="u16"])), const<i32>(7)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_8:[0-9]+]] @lshiftrt_hi_8(%[[VALUE_n_49:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_49]]) [c="u16"])), const<i32>(8)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_9:[0-9]+]] @lshiftrt_hi_9(%[[VALUE_n_50:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_50]]) [c="u16"])), const<i32>(9)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_10:[0-9]+]] @lshiftrt_hi_10(%[[VALUE_n_51:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_51]]) [c="u16"])), const<i32>(10)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_11:[0-9]+]] @lshiftrt_hi_11(%[[VALUE_n_52:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_52]]) [c="u16"])), const<i32>(11)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_12:[0-9]+]] @lshiftrt_hi_12(%[[VALUE_n_53:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_53]]) [c="u16"])), const<i32>(12)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_13:[0-9]+]] @lshiftrt_hi_13(%[[VALUE_n_54:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_54]]) [c="u16"])), const<i32>(13)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_14:[0-9]+]] @lshiftrt_hi_14(%[[VALUE_n_55:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_55]]) [c="u16"])), const<i32>(14)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_hi_15:[0-9]+]] @lshiftrt_hi_15(%[[VALUE_n_56:[0-9]+]] n: u16 [c="u16"]) -> u16 [linkage=external] [fallthrough=ub_if_used] [c="u16(u16)"] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n_56]]) [c="u16"])), const<i32>(15)))) [c="u16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_0:[0-9]+]] @ashiftrt_hi_0(%[[VALUE_n_57:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_57]]) [c="s16"]), const<i32>(0))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_1:[0-9]+]] @ashiftrt_hi_1(%[[VALUE_n_58:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_58]]) [c="s16"]), const<i32>(1))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_2:[0-9]+]] @ashiftrt_hi_2(%[[VALUE_n_59:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_59]]) [c="s16"]), const<i32>(2))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_3:[0-9]+]] @ashiftrt_hi_3(%[[VALUE_n_60:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_60]]) [c="s16"]), const<i32>(3))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_4:[0-9]+]] @ashiftrt_hi_4(%[[VALUE_n_61:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_61]]) [c="s16"]), const<i32>(4))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_5:[0-9]+]] @ashiftrt_hi_5(%[[VALUE_n_62:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_62]]) [c="s16"]), const<i32>(5))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_6:[0-9]+]] @ashiftrt_hi_6(%[[VALUE_n_63:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_63]]) [c="s16"]), const<i32>(6))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_7:[0-9]+]] @ashiftrt_hi_7(%[[VALUE_n_64:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_64]]) [c="s16"]), const<i32>(7))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_8:[0-9]+]] @ashiftrt_hi_8(%[[VALUE_n_65:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_65]]) [c="s16"]), const<i32>(8))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_9:[0-9]+]] @ashiftrt_hi_9(%[[VALUE_n_66:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_66]]) [c="s16"]), const<i32>(9))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_10:[0-9]+]] @ashiftrt_hi_10(%[[VALUE_n_67:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_67]]) [c="s16"]), const<i32>(10))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_11:[0-9]+]] @ashiftrt_hi_11(%[[VALUE_n_68:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_68]]) [c="s16"]), const<i32>(11))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_12:[0-9]+]] @ashiftrt_hi_12(%[[VALUE_n_69:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_69]]) [c="s16"]), const<i32>(12))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_13:[0-9]+]] @ashiftrt_hi_13(%[[VALUE_n_70:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_70]]) [c="s16"]), const<i32>(13))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_14:[0-9]+]] @ashiftrt_hi_14(%[[VALUE_n_71:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_71]]) [c="s16"]), const<i32>(14))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_hi_15:[0-9]+]] @ashiftrt_hi_15(%[[VALUE_n_72:[0-9]+]] n: i16 [c="s16"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c="s16(s16)"] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_72]]) [c="s16"]), const<i32>(15))) [c="s16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_0:[0-9]+]] @ashift_si_0(%[[VALUE_n_73:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_73]]) [c="u32"], const<i32>(0)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_1:[0-9]+]] @ashift_si_1(%[[VALUE_n_74:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_74]]) [c="u32"], const<i32>(1)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_2:[0-9]+]] @ashift_si_2(%[[VALUE_n_75:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_75]]) [c="u32"], const<i32>(2)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_3:[0-9]+]] @ashift_si_3(%[[VALUE_n_76:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_76]]) [c="u32"], const<i32>(3)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_4:[0-9]+]] @ashift_si_4(%[[VALUE_n_77:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_77]]) [c="u32"], const<i32>(4)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_5:[0-9]+]] @ashift_si_5(%[[VALUE_n_78:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_78]]) [c="u32"], const<i32>(5)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_6:[0-9]+]] @ashift_si_6(%[[VALUE_n_79:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_79]]) [c="u32"], const<i32>(6)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_7:[0-9]+]] @ashift_si_7(%[[VALUE_n_80:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_80]]) [c="u32"], const<i32>(7)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_8:[0-9]+]] @ashift_si_8(%[[VALUE_n_81:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_81]]) [c="u32"], const<i32>(8)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_9:[0-9]+]] @ashift_si_9(%[[VALUE_n_82:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_82]]) [c="u32"], const<i32>(9)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_10:[0-9]+]] @ashift_si_10(%[[VALUE_n_83:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_83]]) [c="u32"], const<i32>(10)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_11:[0-9]+]] @ashift_si_11(%[[VALUE_n_84:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_84]]) [c="u32"], const<i32>(11)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_12:[0-9]+]] @ashift_si_12(%[[VALUE_n_85:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_85]]) [c="u32"], const<i32>(12)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_13:[0-9]+]] @ashift_si_13(%[[VALUE_n_86:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_86]]) [c="u32"], const<i32>(13)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_14:[0-9]+]] @ashift_si_14(%[[VALUE_n_87:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_87]]) [c="u32"], const<i32>(14)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_15:[0-9]+]] @ashift_si_15(%[[VALUE_n_88:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_88]]) [c="u32"], const<i32>(15)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_16:[0-9]+]] @ashift_si_16(%[[VALUE_n_89:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_89]]) [c="u32"], const<i32>(16)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_17:[0-9]+]] @ashift_si_17(%[[VALUE_n_90:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_90]]) [c="u32"], const<i32>(17)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_18:[0-9]+]] @ashift_si_18(%[[VALUE_n_91:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_91]]) [c="u32"], const<i32>(18)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_19:[0-9]+]] @ashift_si_19(%[[VALUE_n_92:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_92]]) [c="u32"], const<i32>(19)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_20:[0-9]+]] @ashift_si_20(%[[VALUE_n_93:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_93]]) [c="u32"], const<i32>(20)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_21:[0-9]+]] @ashift_si_21(%[[VALUE_n_94:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_94]]) [c="u32"], const<i32>(21)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_22:[0-9]+]] @ashift_si_22(%[[VALUE_n_95:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_95]]) [c="u32"], const<i32>(22)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_23:[0-9]+]] @ashift_si_23(%[[VALUE_n_96:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_96]]) [c="u32"], const<i32>(23)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_24:[0-9]+]] @ashift_si_24(%[[VALUE_n_97:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_97]]) [c="u32"], const<i32>(24)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_25:[0-9]+]] @ashift_si_25(%[[VALUE_n_98:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_98]]) [c="u32"], const<i32>(25)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_26:[0-9]+]] @ashift_si_26(%[[VALUE_n_99:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_99]]) [c="u32"], const<i32>(26)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_27:[0-9]+]] @ashift_si_27(%[[VALUE_n_100:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_100]]) [c="u32"], const<i32>(27)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_28:[0-9]+]] @ashift_si_28(%[[VALUE_n_101:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_101]]) [c="u32"], const<i32>(28)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_29:[0-9]+]] @ashift_si_29(%[[VALUE_n_102:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_102]]) [c="u32"], const<i32>(29)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_30:[0-9]+]] @ashift_si_30(%[[VALUE_n_103:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_103]]) [c="u32"], const<i32>(30)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashift_si_31:[0-9]+]] @ashift_si_31(%[[VALUE_n_104:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_n_104]]) [c="u32"], const<i32>(31)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_0:[0-9]+]] @lshiftrt_si_0(%[[VALUE_n_105:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_105]]) [c="u32"], const<i32>(0)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_1:[0-9]+]] @lshiftrt_si_1(%[[VALUE_n_106:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_106]]) [c="u32"], const<i32>(1)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_2:[0-9]+]] @lshiftrt_si_2(%[[VALUE_n_107:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_107]]) [c="u32"], const<i32>(2)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_3:[0-9]+]] @lshiftrt_si_3(%[[VALUE_n_108:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_108]]) [c="u32"], const<i32>(3)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_4:[0-9]+]] @lshiftrt_si_4(%[[VALUE_n_109:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_109]]) [c="u32"], const<i32>(4)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_5:[0-9]+]] @lshiftrt_si_5(%[[VALUE_n_110:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_110]]) [c="u32"], const<i32>(5)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_6:[0-9]+]] @lshiftrt_si_6(%[[VALUE_n_111:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_111]]) [c="u32"], const<i32>(6)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_7:[0-9]+]] @lshiftrt_si_7(%[[VALUE_n_112:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_112]]) [c="u32"], const<i32>(7)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_8:[0-9]+]] @lshiftrt_si_8(%[[VALUE_n_113:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_113]]) [c="u32"], const<i32>(8)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_9:[0-9]+]] @lshiftrt_si_9(%[[VALUE_n_114:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_114]]) [c="u32"], const<i32>(9)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_10:[0-9]+]] @lshiftrt_si_10(%[[VALUE_n_115:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_115]]) [c="u32"], const<i32>(10)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_11:[0-9]+]] @lshiftrt_si_11(%[[VALUE_n_116:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_116]]) [c="u32"], const<i32>(11)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_12:[0-9]+]] @lshiftrt_si_12(%[[VALUE_n_117:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_117]]) [c="u32"], const<i32>(12)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_13:[0-9]+]] @lshiftrt_si_13(%[[VALUE_n_118:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_118]]) [c="u32"], const<i32>(13)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_14:[0-9]+]] @lshiftrt_si_14(%[[VALUE_n_119:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_119]]) [c="u32"], const<i32>(14)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_15:[0-9]+]] @lshiftrt_si_15(%[[VALUE_n_120:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_120]]) [c="u32"], const<i32>(15)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_16:[0-9]+]] @lshiftrt_si_16(%[[VALUE_n_121:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_121]]) [c="u32"], const<i32>(16)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_17:[0-9]+]] @lshiftrt_si_17(%[[VALUE_n_122:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_122]]) [c="u32"], const<i32>(17)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_18:[0-9]+]] @lshiftrt_si_18(%[[VALUE_n_123:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_123]]) [c="u32"], const<i32>(18)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_19:[0-9]+]] @lshiftrt_si_19(%[[VALUE_n_124:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_124]]) [c="u32"], const<i32>(19)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_20:[0-9]+]] @lshiftrt_si_20(%[[VALUE_n_125:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_125]]) [c="u32"], const<i32>(20)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_21:[0-9]+]] @lshiftrt_si_21(%[[VALUE_n_126:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_126]]) [c="u32"], const<i32>(21)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_22:[0-9]+]] @lshiftrt_si_22(%[[VALUE_n_127:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_127]]) [c="u32"], const<i32>(22)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_23:[0-9]+]] @lshiftrt_si_23(%[[VALUE_n_128:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_128]]) [c="u32"], const<i32>(23)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_24:[0-9]+]] @lshiftrt_si_24(%[[VALUE_n_129:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_129]]) [c="u32"], const<i32>(24)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_25:[0-9]+]] @lshiftrt_si_25(%[[VALUE_n_130:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_130]]) [c="u32"], const<i32>(25)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_26:[0-9]+]] @lshiftrt_si_26(%[[VALUE_n_131:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_131]]) [c="u32"], const<i32>(26)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_27:[0-9]+]] @lshiftrt_si_27(%[[VALUE_n_132:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_132]]) [c="u32"], const<i32>(27)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_28:[0-9]+]] @lshiftrt_si_28(%[[VALUE_n_133:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_133]]) [c="u32"], const<i32>(28)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_29:[0-9]+]] @lshiftrt_si_29(%[[VALUE_n_134:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_134]]) [c="u32"], const<i32>(29)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_30:[0-9]+]] @lshiftrt_si_30(%[[VALUE_n_135:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_135]]) [c="u32"], const<i32>(30)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lshiftrt_si_31:[0-9]+]] @lshiftrt_si_31(%[[VALUE_n_136:[0-9]+]] n: u32 [c="u32"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c="u32(u32)"] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_n_136]]) [c="u32"], const<i32>(31)) [c="u32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_0:[0-9]+]] @ashiftrt_si_0(%[[VALUE_n_137:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_137]]) [c="s32"], const<i32>(0)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_1:[0-9]+]] @ashiftrt_si_1(%[[VALUE_n_138:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_138]]) [c="s32"], const<i32>(1)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_2:[0-9]+]] @ashiftrt_si_2(%[[VALUE_n_139:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_139]]) [c="s32"], const<i32>(2)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_3:[0-9]+]] @ashiftrt_si_3(%[[VALUE_n_140:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_140]]) [c="s32"], const<i32>(3)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_4:[0-9]+]] @ashiftrt_si_4(%[[VALUE_n_141:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_141]]) [c="s32"], const<i32>(4)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_5:[0-9]+]] @ashiftrt_si_5(%[[VALUE_n_142:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_142]]) [c="s32"], const<i32>(5)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_6:[0-9]+]] @ashiftrt_si_6(%[[VALUE_n_143:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_143]]) [c="s32"], const<i32>(6)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_7:[0-9]+]] @ashiftrt_si_7(%[[VALUE_n_144:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_144]]) [c="s32"], const<i32>(7)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_8:[0-9]+]] @ashiftrt_si_8(%[[VALUE_n_145:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_145]]) [c="s32"], const<i32>(8)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_9:[0-9]+]] @ashiftrt_si_9(%[[VALUE_n_146:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_146]]) [c="s32"], const<i32>(9)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_10:[0-9]+]] @ashiftrt_si_10(%[[VALUE_n_147:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_147]]) [c="s32"], const<i32>(10)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_11:[0-9]+]] @ashiftrt_si_11(%[[VALUE_n_148:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_148]]) [c="s32"], const<i32>(11)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_12:[0-9]+]] @ashiftrt_si_12(%[[VALUE_n_149:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_149]]) [c="s32"], const<i32>(12)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_13:[0-9]+]] @ashiftrt_si_13(%[[VALUE_n_150:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_150]]) [c="s32"], const<i32>(13)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_14:[0-9]+]] @ashiftrt_si_14(%[[VALUE_n_151:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_151]]) [c="s32"], const<i32>(14)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_15:[0-9]+]] @ashiftrt_si_15(%[[VALUE_n_152:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_152]]) [c="s32"], const<i32>(15)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_16:[0-9]+]] @ashiftrt_si_16(%[[VALUE_n_153:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_153]]) [c="s32"], const<i32>(16)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_17:[0-9]+]] @ashiftrt_si_17(%[[VALUE_n_154:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_154]]) [c="s32"], const<i32>(17)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_18:[0-9]+]] @ashiftrt_si_18(%[[VALUE_n_155:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_155]]) [c="s32"], const<i32>(18)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_19:[0-9]+]] @ashiftrt_si_19(%[[VALUE_n_156:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_156]]) [c="s32"], const<i32>(19)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_20:[0-9]+]] @ashiftrt_si_20(%[[VALUE_n_157:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_157]]) [c="s32"], const<i32>(20)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_21:[0-9]+]] @ashiftrt_si_21(%[[VALUE_n_158:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_158]]) [c="s32"], const<i32>(21)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_22:[0-9]+]] @ashiftrt_si_22(%[[VALUE_n_159:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_159]]) [c="s32"], const<i32>(22)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_23:[0-9]+]] @ashiftrt_si_23(%[[VALUE_n_160:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_160]]) [c="s32"], const<i32>(23)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_24:[0-9]+]] @ashiftrt_si_24(%[[VALUE_n_161:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_161]]) [c="s32"], const<i32>(24)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_25:[0-9]+]] @ashiftrt_si_25(%[[VALUE_n_162:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_162]]) [c="s32"], const<i32>(25)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_26:[0-9]+]] @ashiftrt_si_26(%[[VALUE_n_163:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_163]]) [c="s32"], const<i32>(26)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_27:[0-9]+]] @ashiftrt_si_27(%[[VALUE_n_164:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_164]]) [c="s32"], const<i32>(27)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_28:[0-9]+]] @ashiftrt_si_28(%[[VALUE_n_165:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_165]]) [c="s32"], const<i32>(28)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_29:[0-9]+]] @ashiftrt_si_29(%[[VALUE_n_166:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_166]]) [c="s32"], const<i32>(29)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_30:[0-9]+]] @ashiftrt_si_30(%[[VALUE_n_167:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_167]]) [c="s32"], const<i32>(30)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ashiftrt_si_31:[0-9]+]] @ashiftrt_si_31(%[[VALUE_n_168:[0-9]+]] n: i32 [c="s32"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="s32(s32)"] {
// DEFAULT-NEXT:         return shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_n_168]]) [c="s32"], const<i32>(31)) [c="s32"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_ashift_qi_0]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(0)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_ashift_qi_1]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(1)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_ashift_qi_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(2)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_ashift_qi_3]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(3)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_ashift_qi_4]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(4)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_ashift_qi_5]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(5)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_ashift_qi_6]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(6)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_ashift_qi_7]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(7)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_lshiftrt_qi_0]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(0)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_lshiftrt_qi_1]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(1)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_lshiftrt_qi_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(2)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_lshiftrt_qi_3]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(3)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_lshiftrt_qi_4]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(4)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_lshiftrt_qi_5]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(5)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_lshiftrt_qi_6]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(6)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE_lshiftrt_qi_7]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(255))) [c="u8"]) [c="u8"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))) [c="u8"])), const<i32>(7)))) [c="u8"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_0]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s8"]) [c="s8"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_1]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s8"]) [c="s8"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_2]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s8"]) [c="s8"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_3]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s8"]) [c="s8"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_4]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s8"]) [c="s8"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_5]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s8"]) [c="s8"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_6]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s8"]) [c="s8"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_7]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s8"]) [c="s8"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_0]], truncate<i8, reason=arg, fits=always>(const<i32>(0)) [c="s8"]) [c="s8"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_1]], truncate<i8, reason=arg, fits=always>(const<i32>(0)) [c="s8"]) [c="s8"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_2]], truncate<i8, reason=arg, fits=always>(const<i32>(0)) [c="s8"]) [c="s8"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_3]], truncate<i8, reason=arg, fits=always>(const<i32>(0)) [c="s8"]) [c="s8"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_4]], truncate<i8, reason=arg, fits=always>(const<i32>(0)) [c="s8"]) [c="s8"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_5]], truncate<i8, reason=arg, fits=always>(const<i32>(0)) [c="s8"]) [c="s8"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_6]], truncate<i8, reason=arg, fits=always>(const<i32>(0)) [c="s8"]) [c="s8"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8) -> i8>(%[[VALUE_ashiftrt_qi_7]], truncate<i8, reason=arg, fits=always>(const<i32>(0)) [c="s8"]) [c="s8"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_0]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(0)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_1]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(1)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_2]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(2)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_3]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(3)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_4]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(4)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_5]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(5)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_6]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(6)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_7]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(7)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_8]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(8)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_9]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(9)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_10]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(10)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_11]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(11)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_12]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(12)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_13]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(13)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_14]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(14)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_ashift_hi_15]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(15)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_0]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(0)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_1]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(1)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_2]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(2)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_3]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(3)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_4]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(4)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_5]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(5)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_6]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(6)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_7]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(7)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_8]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(8)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_9]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(9)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_10]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(10)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_11]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(11)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_12]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(12)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_13]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(13)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_14]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(14)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_lshiftrt_hi_15]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))) [c="u16"]) [c="u16"])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))) [c="u16"])), const<i32>(15)))) [c="u16"])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_0]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_1]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_2]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_3]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_4]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_5]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_6]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_7]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_8]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_9]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_10]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_11]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_12]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_13]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_14]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_15]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [c="s16"]) [c="s16"]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_0]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_1]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_2]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_3]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_4]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_5]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_6]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_7]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_8]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_9]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_10]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_11]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_12]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_13]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_14]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_ashiftrt_hi_15]], truncate<i16, reason=arg, fits=always>(const<i32>(0)) [c="s16"]) [c="s16"]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_0]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(0)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_1]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(1)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_2]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(2)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_3]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(3)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_4]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(4)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_5]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(5)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_6]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(6)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_7]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(7)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_8]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(8)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_9]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(9)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_10]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(10)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_11]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(11)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_12]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(12)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_13]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(13)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_14]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(14)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_15]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(15)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_16]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(16)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_17]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(17)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_18]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(18)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_19]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(19)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_20]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(20)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_21]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(21)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_22]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(22)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_23]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(23)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_24]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(24)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_25]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(25)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_26]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(26)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_27]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(27)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_28]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(28)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_29]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(29)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_30]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(30)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_ashift_si_31]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(31)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_0]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(0)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_1]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(1)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_2]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(2)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_3]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(3)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_4]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(4)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_5]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(5)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_6]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(6)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_7]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(7)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_8]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(8)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_9]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(9)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_10]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(10)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_11]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(11)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_12]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(12)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_13]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(13)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_14]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(14)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_15]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(15)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_16]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(16)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_17]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(17)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_18]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(18)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_19]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(19)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_20]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(20)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_21]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(21)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_22]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(22)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_23]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(23)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_24]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(24)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_25]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(25)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_26]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(26)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_27]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(27)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_28]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(28)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_29]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(29)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_30]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(30)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_lshiftrt_si_31]], reinterpret<u32, reason=arg>(const<u32>(4294967295)) [c="u32"]) [c="u32"], shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit>(const<u32>(4294967295)) [c="u32"], const<i32>(31)) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_0]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_1]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_2]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_3]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_4]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_5]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_6]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_7]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_8]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_9]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_10]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_11]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_12]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_13]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_14]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_15]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_16]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_17]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_18]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_19]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_20]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_21]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_22]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_23]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_24]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_25]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_26]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_27]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_28]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_29]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_30]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_31]], reinterpret<i32, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_0]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_1]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_2]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_3]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_4]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_5]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_6]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_7]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_8]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_9]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_10]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_11]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_12]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_13]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_14]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_15]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_16]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_17]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_18]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_19]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_20]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_21]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_22]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_23]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_24]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_25]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_26]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_27]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_28]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_29]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_30]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_ashiftrt_si_31]], reinterpret<i32, reason=arg>(const<i32>(0)) [c="s32"]) [c="s32"], reinterpret<i32, reason=usual_arith>(const<i32>(0)) [c="s32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
