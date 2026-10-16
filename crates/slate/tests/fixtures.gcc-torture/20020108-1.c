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

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This file tests shifts in various integral modes.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Define 8-bit shifts.
// CHECK-NEXT: fn ashift_qi_0(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) << (0 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_qi_1(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) << (1 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_qi_2(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) << (2 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_qi_3(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) << (3 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_qi_4(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) << (4 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_qi_5(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) << (5 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_qi_6(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) << (6 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_qi_7(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) << (7 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_qi_0(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (0 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_qi_1(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (1 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_qi_2(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (2 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_qi_3(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (3 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_qi_4(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (4 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_qi_5(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (5 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_qi_6(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (6 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_qi_7(mut n: u8) -> u8 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (7 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_qi_0(mut n: i8) -> i8 {
// CHECK-NEXT:     ((n as i32) >> (0 as i32)) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_qi_1(mut n: i8) -> i8 {
// CHECK-NEXT:     ((n as i32) >> (1 as i32)) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_qi_2(mut n: i8) -> i8 {
// CHECK-NEXT:     ((n as i32) >> (2 as i32)) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_qi_3(mut n: i8) -> i8 {
// CHECK-NEXT:     ((n as i32) >> (3 as i32)) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_qi_4(mut n: i8) -> i8 {
// CHECK-NEXT:     ((n as i32) >> (4 as i32)) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_qi_5(mut n: i8) -> i8 {
// CHECK-NEXT:     ((n as i32) >> (5 as i32)) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_qi_6(mut n: i8) -> i8 {
// CHECK-NEXT:     ((n as i32) >> (6 as i32)) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_qi_7(mut n: i8) -> i8 {
// CHECK-NEXT:     ((n as i32) >> (7 as i32)) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Define 16-bit shifts.
// CHECK-NEXT: fn ashift_hi_0(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (0 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_1(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (1 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_2(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (2 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_3(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (3 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_4(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (4 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_5(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (5 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_6(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (6 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_7(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (7 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_8(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (8 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_9(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (9 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_10(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (10 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_11(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (11 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_12(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (12 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_13(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (13 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_14(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (14 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_hi_15(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) << (15 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_0(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (0 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_1(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (1 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_2(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (2 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_3(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (3 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_4(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (4 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_5(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (5 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_6(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (6 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_7(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (7 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_8(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (8 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_9(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (9 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_10(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (10 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_11(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (11 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_12(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (12 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_13(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (13 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_14(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (14 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_hi_15(mut n: u16) -> u16 {
// CHECK-NEXT:     ((((n as u32) as i32) >> (15 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_0(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (0 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_1(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (1 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_2(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (2 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_3(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (3 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_4(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (4 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_5(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (5 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_6(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (6 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_7(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (7 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_8(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (8 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_9(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (9 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_10(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (10 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_11(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (11 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_12(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (12 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_13(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (13 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_14(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (14 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_hi_15(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) >> (15 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Define 32-bit shifts.
// CHECK-NEXT: fn ashift_si_0(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (0 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_1(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (1 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_2(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (2 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_3(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (3 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_4(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (4 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_5(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (5 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_6(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (6 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_7(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (7 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_8(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (8 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_9(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (9 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_10(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (10 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_11(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (11 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_12(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (12 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_13(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (13 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_14(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (14 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_15(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (15 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_16(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (16 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_17(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (17 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_18(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (18 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_19(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (19 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_20(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (20 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_21(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (21 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_22(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (22 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_23(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (23 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_24(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (24 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_25(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (25 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_26(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (26 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_27(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (27 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_28(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (28 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_29(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (29 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_30(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (30 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashift_si_31(mut n: u32) -> u32 {
// CHECK-NEXT:     n << (31 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_0(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (0 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_1(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (1 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_2(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (2 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_3(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (3 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_4(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (4 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_5(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (5 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_6(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (6 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_7(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (7 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_8(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (8 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_9(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (9 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_10(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (10 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_11(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (11 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_12(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (12 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_13(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (13 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_14(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (14 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_15(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (15 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_16(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (16 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_17(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (17 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_18(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (18 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_19(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (19 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_20(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (20 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_21(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (21 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_22(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (22 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_23(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (23 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_24(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (24 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_25(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (25 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_26(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (26 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_27(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (27 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_28(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (28 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_29(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (29 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_30(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (30 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lshiftrt_si_31(mut n: u32) -> u32 {
// CHECK-NEXT:     n >> (31 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_0(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (0 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_1(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (1 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_2(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (2 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_3(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (3 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_4(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (4 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_5(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (5 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_6(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (6 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_7(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (7 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_8(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (8 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_9(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (9 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_10(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (10 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_11(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (11 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_12(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (12 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_13(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (13 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_14(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (14 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_15(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (15 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_16(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (16 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_17(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (17 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_18(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (18 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_19(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (19 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_20(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (20 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_21(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (21 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_22(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (22 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_23(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (23 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_24(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (24 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_25(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (25 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_26(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (26 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_27(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (27 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_28(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (28 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_29(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (29 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_30(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (30 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ashiftrt_si_31(mut n: i32) -> i32 {
// CHECK-NEXT:     n >> (31 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // Test 8-bit shifts.
// CHECK-NEXT:     if ((ashift_qi_0(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) << (0 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_qi_1(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) << (1 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_qi_2(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) << (2 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_qi_3(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) << (3 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_qi_4(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) << (4 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_qi_5(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) << (5 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_qi_6(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) << (6 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_qi_7(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) << (7 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ((lshiftrt_qi_0(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) >> (0 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_qi_1(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) >> (1 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_qi_2(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) >> (2 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_qi_3(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) >> (3 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_qi_4(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) >> (4 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_qi_5(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) >> (5 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_qi_6(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) >> (6 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_qi_7(((255 as i32) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         != ((((((((((255 as i32) as i8) as u8) as u32) as i32) >> (7 as i32)) as i8) as u8) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if (ashiftrt_qi_0(-(1 as i32) as i8) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_1(-(1 as i32) as i8) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_2(-(1 as i32) as i8) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_3(-(1 as i32) as i8) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_4(-(1 as i32) as i8) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_5(-(1 as i32) as i8) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_6(-(1 as i32) as i8) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_7(-(1 as i32) as i8) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if (ashiftrt_qi_0((0 as i32) as i8) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_1((0 as i32) as i8) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_2((0 as i32) as i8) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_3((0 as i32) as i8) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_4((0 as i32) as i8) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_5((0 as i32) as i8) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_6((0 as i32) as i8) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_qi_7((0 as i32) as i8) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Test 16-bit shifts.
// CHECK-NEXT:     if ((ashift_hi_0(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (0 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_1(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (1 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_2(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (2 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_3(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (3 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_4(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (4 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_5(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (5 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_6(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (6 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_7(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (7 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_8(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (8 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_9(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (9 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_10(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (10 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_11(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (11 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_12(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (12 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_13(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (13 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_14(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (14 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((ashift_hi_15(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) << (15 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ((lshiftrt_hi_0(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (0 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_1(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (1 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_2(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (2 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_3(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (3 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_4(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (4 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_5(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (5 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_6(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (6 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_7(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (7 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_8(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (8 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_9(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (9 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_10(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (10 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_11(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (11 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_12(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (12 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_13(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (13 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_14(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (14 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((lshiftrt_hi_15(((65535 as i32) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         != ((((((((((65535 as i32) as i16) as u16) as u32) as i32) >> (15 as i32)) as i16) as u16)
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if (ashiftrt_hi_0(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_1(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_2(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_3(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_4(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_5(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_6(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_7(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_8(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_9(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_10(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_11(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_12(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_13(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_14(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_15(-(1 as i32) as i16) as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if (ashiftrt_hi_0((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_1((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_2((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_3((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_4((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_5((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_6((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_7((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_8((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_9((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_10((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_11((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_12((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_13((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_14((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (ashiftrt_hi_15((0 as i32) as i16) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Test 32-bit shifts.
// CHECK-NEXT:     if ashift_si_0(4294967295 as u32) != (4294967295 as u32) << (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_1(4294967295 as u32) != (4294967295 as u32) << (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_2(4294967295 as u32) != (4294967295 as u32) << (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_3(4294967295 as u32) != (4294967295 as u32) << (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_4(4294967295 as u32) != (4294967295 as u32) << (4 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_5(4294967295 as u32) != (4294967295 as u32) << (5 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_6(4294967295 as u32) != (4294967295 as u32) << (6 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_7(4294967295 as u32) != (4294967295 as u32) << (7 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_8(4294967295 as u32) != (4294967295 as u32) << (8 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_9(4294967295 as u32) != (4294967295 as u32) << (9 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_10(4294967295 as u32) != (4294967295 as u32) << (10 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_11(4294967295 as u32) != (4294967295 as u32) << (11 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_12(4294967295 as u32) != (4294967295 as u32) << (12 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_13(4294967295 as u32) != (4294967295 as u32) << (13 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_14(4294967295 as u32) != (4294967295 as u32) << (14 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_15(4294967295 as u32) != (4294967295 as u32) << (15 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_16(4294967295 as u32) != (4294967295 as u32) << (16 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_17(4294967295 as u32) != (4294967295 as u32) << (17 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_18(4294967295 as u32) != (4294967295 as u32) << (18 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_19(4294967295 as u32) != (4294967295 as u32) << (19 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_20(4294967295 as u32) != (4294967295 as u32) << (20 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_21(4294967295 as u32) != (4294967295 as u32) << (21 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_22(4294967295 as u32) != (4294967295 as u32) << (22 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_23(4294967295 as u32) != (4294967295 as u32) << (23 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_24(4294967295 as u32) != (4294967295 as u32) << (24 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_25(4294967295 as u32) != (4294967295 as u32) << (25 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_26(4294967295 as u32) != (4294967295 as u32) << (26 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_27(4294967295 as u32) != (4294967295 as u32) << (27 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_28(4294967295 as u32) != (4294967295 as u32) << (28 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_29(4294967295 as u32) != (4294967295 as u32) << (29 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_30(4294967295 as u32) != (4294967295 as u32) << (30 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashift_si_31(4294967295 as u32) != (4294967295 as u32) << (31 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if lshiftrt_si_0(4294967295 as u32) != (4294967295 as u32) >> (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_1(4294967295 as u32) != (4294967295 as u32) >> (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_2(4294967295 as u32) != (4294967295 as u32) >> (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_3(4294967295 as u32) != (4294967295 as u32) >> (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_4(4294967295 as u32) != (4294967295 as u32) >> (4 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_5(4294967295 as u32) != (4294967295 as u32) >> (5 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_6(4294967295 as u32) != (4294967295 as u32) >> (6 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_7(4294967295 as u32) != (4294967295 as u32) >> (7 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_8(4294967295 as u32) != (4294967295 as u32) >> (8 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_9(4294967295 as u32) != (4294967295 as u32) >> (9 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_10(4294967295 as u32) != (4294967295 as u32) >> (10 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_11(4294967295 as u32) != (4294967295 as u32) >> (11 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_12(4294967295 as u32) != (4294967295 as u32) >> (12 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_13(4294967295 as u32) != (4294967295 as u32) >> (13 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_14(4294967295 as u32) != (4294967295 as u32) >> (14 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_15(4294967295 as u32) != (4294967295 as u32) >> (15 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_16(4294967295 as u32) != (4294967295 as u32) >> (16 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_17(4294967295 as u32) != (4294967295 as u32) >> (17 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_18(4294967295 as u32) != (4294967295 as u32) >> (18 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_19(4294967295 as u32) != (4294967295 as u32) >> (19 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_20(4294967295 as u32) != (4294967295 as u32) >> (20 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_21(4294967295 as u32) != (4294967295 as u32) >> (21 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_22(4294967295 as u32) != (4294967295 as u32) >> (22 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_23(4294967295 as u32) != (4294967295 as u32) >> (23 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_24(4294967295 as u32) != (4294967295 as u32) >> (24 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_25(4294967295 as u32) != (4294967295 as u32) >> (25 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_26(4294967295 as u32) != (4294967295 as u32) >> (26 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_27(4294967295 as u32) != (4294967295 as u32) >> (27 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_28(4294967295 as u32) != (4294967295 as u32) >> (28 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_29(4294967295 as u32) != (4294967295 as u32) >> (29 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_30(4294967295 as u32) != (4294967295 as u32) >> (30 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if lshiftrt_si_31(4294967295 as u32) != (4294967295 as u32) >> (31 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ashiftrt_si_0(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_1(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_2(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_3(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_4(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_5(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_6(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_7(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_8(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_9(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_10(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_11(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_12(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_13(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_14(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_15(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_16(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_17(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_18(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_19(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_20(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_21(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_22(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_23(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_24(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_25(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_26(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_27(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_28(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_29(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_30(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_31(-(1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ashiftrt_si_0(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_1(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_2(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_3(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_4(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_5(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_6(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_7(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_8(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_9(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_10(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_11(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_12(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_13(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_14(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_15(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_16(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_17(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_18(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_19(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_20(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_21(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_22(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_23(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_24(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_25(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_26(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_27(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_28(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_29(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_30(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ashiftrt_si_31(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
