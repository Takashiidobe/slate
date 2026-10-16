#include <limits.h>

void abort(void);
void exit(int);

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#define ROR(a, b) (((a) >> (b)) | ((a) << ((sizeof(a) * CHAR_BIT) - (b))))
#define ROL(a, b) (((a) << (b)) | ((a) >> ((sizeof(a) * CHAR_BIT) - (b))))

#define CHAR_VALUE  ((char)0xf234)
#define SHORT_VALUE ((short)0xf234)
#define INT_VALUE   ((int)0xf234)
#define LONG_VALUE  ((long)0xf2345678L)
#define LL_VALUE    ((long long)0xf2345678abcdef0LL)

#define SHIFT1 4
#define SHIFT2 ((sizeof(long long) * CHAR_BIT) - SHIFT1)

char      c      = CHAR_VALUE;
short     s      = SHORT_VALUE;
int       i      = INT_VALUE;
long      l      = LONG_VALUE;
long long ll     = LL_VALUE;
int       shift1 = SHIFT1;
int       shift2 = SHIFT2;

int main(void) {
  if (ROR(c, shift1) != ROR(CHAR_VALUE, SHIFT1))
    abort();

  if (ROR(c, SHIFT1) != ROR(CHAR_VALUE, SHIFT1))
    abort();

  if (ROR(s, shift1) != ROR(SHORT_VALUE, SHIFT1))
    abort();

  if (ROR(s, SHIFT1) != ROR(SHORT_VALUE, SHIFT1))
    abort();

  if (ROR(i, shift1) != ROR(INT_VALUE, SHIFT1))
    abort();

  if (ROR(i, SHIFT1) != ROR(INT_VALUE, SHIFT1))
    abort();

  if (ROR(l, shift1) != ROR(LONG_VALUE, SHIFT1))
    abort();

  if (ROR(l, SHIFT1) != ROR(LONG_VALUE, SHIFT1))
    abort();

  if (ROR(ll, shift1) != ROR(LL_VALUE, SHIFT1))
    abort();

  if (ROR(ll, SHIFT1) != ROR(LL_VALUE, SHIFT1))
    abort();

  if (ROR(ll, shift2) != ROR(LL_VALUE, SHIFT2))
    abort();

  if (ROR(ll, SHIFT2) != ROR(LL_VALUE, SHIFT2))
    abort();

  if (ROL(c, shift1) != ROL(CHAR_VALUE, SHIFT1))
    abort();

  if (ROL(c, SHIFT1) != ROL(CHAR_VALUE, SHIFT1))
    abort();

  if (ROL(s, shift1) != ROL(SHORT_VALUE, SHIFT1))
    abort();

  if (ROL(s, SHIFT1) != ROL(SHORT_VALUE, SHIFT1))
    abort();

  if (ROL(i, shift1) != ROL(INT_VALUE, SHIFT1))
    abort();

  if (ROL(i, SHIFT1) != ROL(INT_VALUE, SHIFT1))
    abort();

  if (ROL(l, shift1) != ROL(LONG_VALUE, SHIFT1))
    abort();

  if (ROL(l, SHIFT1) != ROL(LONG_VALUE, SHIFT1))
    abort();

  if (ROL(ll, shift1) != ROL(LL_VALUE, SHIFT1))
    abort();

  if (ROL(ll, SHIFT1) != ROL(LL_VALUE, SHIFT1))
    abort();

  if (ROL(ll, shift2) != ROL(LL_VALUE, SHIFT2))
    abort();

  if (ROL(ll, SHIFT2) != ROL(LL_VALUE, SHIFT2))
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut c: i8 = (62004 as i32) as i8;
// CHECK-NEXT: static mut s: i16 = (62004 as i32) as i16;
// CHECK-NEXT: static mut i: i32 = 62004 as i32;
// CHECK-NEXT: static mut l: i64 = 4063516280 as i64;
// CHECK-NEXT: static mut ll: i64 = 1090791845765373680 as i64;
// CHECK-NEXT: static mut shift1: i32 = 4 as i32;
// CHECK-NEXT: static mut shift2: i32 = ((8 as u64)
// CHECK-NEXT:     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:     .wrapping_sub(((4 as i32) as i64) as u64) as u32) as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((unsafe { c }) as i32) >> unsafe { shift1 }
// CHECK-NEXT:         | ((unsafe { c }) as i32)
// CHECK-NEXT:             << (1 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (((62004 as i32) as i8) as i32) >> (4 as i32)
// CHECK-NEXT:             | (((62004 as i32) as i8) as i32)
// CHECK-NEXT:                 << (1 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { c }) as i32) >> (4 as i32)
// CHECK-NEXT:         | ((unsafe { c }) as i32)
// CHECK-NEXT:             << (1 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (((62004 as i32) as i8) as i32) >> (4 as i32)
// CHECK-NEXT:             | (((62004 as i32) as i8) as i32)
// CHECK-NEXT:                 << (1 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { s }) as i32) >> unsafe { shift1 }
// CHECK-NEXT:         | ((unsafe { s }) as i32)
// CHECK-NEXT:             << (2 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (((62004 as i32) as i16) as i32) >> (4 as i32)
// CHECK-NEXT:             | (((62004 as i32) as i16) as i32)
// CHECK-NEXT:                 << (2 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { s }) as i32) >> (4 as i32)
// CHECK-NEXT:         | ((unsafe { s }) as i32)
// CHECK-NEXT:             << (2 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (((62004 as i32) as i16) as i32) >> (4 as i32)
// CHECK-NEXT:             | (((62004 as i32) as i16) as i32)
// CHECK-NEXT:                 << (2 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { i }) >> unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { i })
// CHECK-NEXT:             << (4 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (62004 as i32) >> (4 as i32)
// CHECK-NEXT:             | (62004 as i32)
// CHECK-NEXT:                 << (4 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { i }) >> (4 as i32)
// CHECK-NEXT:         | (unsafe { i })
// CHECK-NEXT:             << (4 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (62004 as i32) >> (4 as i32)
// CHECK-NEXT:             | (62004 as i32)
// CHECK-NEXT:                 << (4 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { l }) >> unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { l })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (4063516280 as i64) >> (4 as i32)
// CHECK-NEXT:             | (4063516280 as i64)
// CHECK-NEXT:                 << (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { l }) >> (4 as i32)
// CHECK-NEXT:         | (unsafe { l })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (4063516280 as i64) >> (4 as i32)
// CHECK-NEXT:             | (4063516280 as i64)
// CHECK-NEXT:                 << (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ll }) >> unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { ll })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (1090791845765373680 as i64) >> (4 as i32)
// CHECK-NEXT:             | (1090791845765373680 as i64)
// CHECK-NEXT:                 << (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ll }) >> (4 as i32)
// CHECK-NEXT:         | (unsafe { ll })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (1090791845765373680 as i64) >> (4 as i32)
// CHECK-NEXT:             | (1090791845765373680 as i64)
// CHECK-NEXT:                 << (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ll }) >> unsafe { shift2 }
// CHECK-NEXT:         | (unsafe { ll })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift2 }) as i64) as u64)
// CHECK-NEXT:         != (1090791845765373680 as i64)
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:             | (1090791845765373680 as i64)
// CHECK-NEXT:                 << (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(
// CHECK-NEXT:                         (8 as u64)
// CHECK-NEXT:                             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                             .wrapping_sub(((4 as i32) as i64) as u64),
// CHECK-NEXT:                     )
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ll })
// CHECK-NEXT:         >> (8 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         | (unsafe { ll })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(
// CHECK-NEXT:                     (8 as u64)
// CHECK-NEXT:                         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_sub(((4 as i32) as i64) as u64),
// CHECK-NEXT:                 )
// CHECK-NEXT:         != (1090791845765373680 as i64)
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:             | (1090791845765373680 as i64)
// CHECK-NEXT:                 << (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(
// CHECK-NEXT:                         (8 as u64)
// CHECK-NEXT:                             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                             .wrapping_sub(((4 as i32) as i64) as u64),
// CHECK-NEXT:                     )
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { c }) as i32) << unsafe { shift1 }
// CHECK-NEXT:         | ((unsafe { c }) as i32)
// CHECK-NEXT:             >> (1 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (((62004 as i32) as i8) as i32) << (4 as i32)
// CHECK-NEXT:             | (((62004 as i32) as i8) as i32)
// CHECK-NEXT:                 >> (1 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { c }) as i32) << (4 as i32)
// CHECK-NEXT:         | ((unsafe { c }) as i32)
// CHECK-NEXT:             >> (1 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (((62004 as i32) as i8) as i32) << (4 as i32)
// CHECK-NEXT:             | (((62004 as i32) as i8) as i32)
// CHECK-NEXT:                 >> (1 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { s }) as i32) << unsafe { shift1 }
// CHECK-NEXT:         | ((unsafe { s }) as i32)
// CHECK-NEXT:             >> (2 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (((62004 as i32) as i16) as i32) << (4 as i32)
// CHECK-NEXT:             | (((62004 as i32) as i16) as i32)
// CHECK-NEXT:                 >> (2 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { s }) as i32) << (4 as i32)
// CHECK-NEXT:         | ((unsafe { s }) as i32)
// CHECK-NEXT:             >> (2 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (((62004 as i32) as i16) as i32) << (4 as i32)
// CHECK-NEXT:             | (((62004 as i32) as i16) as i32)
// CHECK-NEXT:                 >> (2 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { i }) << unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { i })
// CHECK-NEXT:             >> (4 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (62004 as i32) << (4 as i32)
// CHECK-NEXT:             | (62004 as i32)
// CHECK-NEXT:                 >> (4 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { i }) << (4 as i32)
// CHECK-NEXT:         | (unsafe { i })
// CHECK-NEXT:             >> (4 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (62004 as i32) << (4 as i32)
// CHECK-NEXT:             | (62004 as i32)
// CHECK-NEXT:                 >> (4 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { l }) << unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { l })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (4063516280 as i64) << (4 as i32)
// CHECK-NEXT:             | (4063516280 as i64)
// CHECK-NEXT:                 >> (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { l }) << (4 as i32)
// CHECK-NEXT:         | (unsafe { l })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (4063516280 as i64) << (4 as i32)
// CHECK-NEXT:             | (4063516280 as i64)
// CHECK-NEXT:                 >> (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ll }) << unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { ll })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (1090791845765373680 as i64) << (4 as i32)
// CHECK-NEXT:             | (1090791845765373680 as i64)
// CHECK-NEXT:                 >> (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ll }) << (4 as i32)
// CHECK-NEXT:         | (unsafe { ll })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (1090791845765373680 as i64) << (4 as i32)
// CHECK-NEXT:             | (1090791845765373680 as i64)
// CHECK-NEXT:                 >> (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ll }) << unsafe { shift2 }
// CHECK-NEXT:         | (unsafe { ll })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift2 }) as i64) as u64)
// CHECK-NEXT:         != (1090791845765373680 as i64)
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:             | (1090791845765373680 as i64)
// CHECK-NEXT:                 >> (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(
// CHECK-NEXT:                         (8 as u64)
// CHECK-NEXT:                             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                             .wrapping_sub(((4 as i32) as i64) as u64),
// CHECK-NEXT:                     )
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ll })
// CHECK-NEXT:         << (8 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         | (unsafe { ll })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(
// CHECK-NEXT:                     (8 as u64)
// CHECK-NEXT:                         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_sub(((4 as i32) as i64) as u64),
// CHECK-NEXT:                 )
// CHECK-NEXT:         != (1090791845765373680 as i64)
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:             | (1090791845765373680 as i64)
// CHECK-NEXT:                 >> (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(
// CHECK-NEXT:                         (8 as u64)
// CHECK-NEXT:                             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                             .wrapping_sub(((4 as i32) as i64) as u64),
// CHECK-NEXT:                     )
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
