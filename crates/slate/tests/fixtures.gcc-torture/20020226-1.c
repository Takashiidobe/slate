/* This tests the rotate patterns that some machines support.  */

#include <limits.h>

void abort(void);
void exit(int);

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#define ROR(a, b) (((a) >> (b)) | ((a) << ((sizeof(a) * CHAR_BIT) - (b))))
#define ROL(a, b) (((a) << (b)) | ((a) >> ((sizeof(a) * CHAR_BIT) - (b))))

#define CHAR_VALUE  ((unsigned char)0x1234U)
#define SHORT_VALUE ((unsigned short)0x1234U)
#define INT_VALUE   0x1234U
#define LONG_VALUE  0x12345678LU
#define LL_VALUE    0x12345678abcdef0LLU

#define SHIFT1 4
#define SHIFT2 ((sizeof(long long) * CHAR_BIT) - SHIFT1)

unsigned char      uc     = CHAR_VALUE;
unsigned short     us     = SHORT_VALUE;
unsigned int       ui     = INT_VALUE;
unsigned long      ul     = LONG_VALUE;
unsigned long long ull    = LL_VALUE;
int                shift1 = SHIFT1;
int                shift2 = SHIFT2;

int main(void) {
  if (ROR(uc, shift1) != ROR(CHAR_VALUE, SHIFT1))
    abort();

  if (ROR(uc, SHIFT1) != ROR(CHAR_VALUE, SHIFT1))
    abort();

  if (ROR(us, shift1) != ROR(SHORT_VALUE, SHIFT1))
    abort();

  if (ROR(us, SHIFT1) != ROR(SHORT_VALUE, SHIFT1))
    abort();

  if (ROR(ui, shift1) != ROR(INT_VALUE, SHIFT1))
    abort();

  if (ROR(ui, SHIFT1) != ROR(INT_VALUE, SHIFT1))
    abort();

  if (ROR(ul, shift1) != ROR(LONG_VALUE, SHIFT1))
    abort();

  if (ROR(ul, SHIFT1) != ROR(LONG_VALUE, SHIFT1))
    abort();

  if (ROR(ull, shift1) != ROR(LL_VALUE, SHIFT1))
    abort();

  if (ROR(ull, SHIFT1) != ROR(LL_VALUE, SHIFT1))
    abort();

  if (ROR(ull, shift2) != ROR(LL_VALUE, SHIFT2))
    abort();

  if (ROR(ull, SHIFT2) != ROR(LL_VALUE, SHIFT2))
    abort();

  if (ROL(uc, shift1) != ROL(CHAR_VALUE, SHIFT1))
    abort();

  if (ROL(uc, SHIFT1) != ROL(CHAR_VALUE, SHIFT1))
    abort();

  if (ROL(us, shift1) != ROL(SHORT_VALUE, SHIFT1))
    abort();

  if (ROL(us, SHIFT1) != ROL(SHORT_VALUE, SHIFT1))
    abort();

  if (ROL(ui, shift1) != ROL(INT_VALUE, SHIFT1))
    abort();

  if (ROL(ui, SHIFT1) != ROL(INT_VALUE, SHIFT1))
    abort();

  if (ROL(ul, shift1) != ROL(LONG_VALUE, SHIFT1))
    abort();

  if (ROL(ul, SHIFT1) != ROL(LONG_VALUE, SHIFT1))
    abort();

  if (ROL(ull, shift1) != ROL(LL_VALUE, SHIFT1))
    abort();

  if (ROL(ull, SHIFT1) != ROL(LL_VALUE, SHIFT1))
    abort();

  if (ROL(ull, shift2) != ROL(LL_VALUE, SHIFT2))
    abort();

  if (ROL(ull, SHIFT2) != ROL(LL_VALUE, SHIFT2))
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This tests the rotate patterns that some machines support.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut uc: u8 = (4660 as u32) as u8;
// CHECK-EMPTY:
// CHECK-NEXT: static mut us: u16 = (4660 as u32) as u16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ui: u32 = 4660 as u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ul: u64 = 305419896 as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ull: u64 = 81985529234382576 as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut shift1: i32 = 4 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut shift2: i32 = ((8 as u64)
// CHECK-NEXT:     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:     .wrapping_sub(((4 as i32) as i64) as u64) as u32) as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (((unsafe { uc }) as u32) as i32) >> unsafe { shift1 }
// CHECK-NEXT:         | (((unsafe { uc }) as u32) as i32)
// CHECK-NEXT:             << (1 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != ((((4660 as u32) as u8) as u32) as i32) >> (4 as i32)
// CHECK-NEXT:             | ((((4660 as u32) as u8) as u32) as i32)
// CHECK-NEXT:                 << (1 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { uc }) as u32) as i32) >> (4 as i32)
// CHECK-NEXT:         | (((unsafe { uc }) as u32) as i32)
// CHECK-NEXT:             << (1 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != ((((4660 as u32) as u8) as u32) as i32) >> (4 as i32)
// CHECK-NEXT:             | ((((4660 as u32) as u8) as u32) as i32)
// CHECK-NEXT:                 << (1 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { us }) as u32) as i32) >> unsafe { shift1 }
// CHECK-NEXT:         | (((unsafe { us }) as u32) as i32)
// CHECK-NEXT:             << (2 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != ((((4660 as u32) as u16) as u32) as i32) >> (4 as i32)
// CHECK-NEXT:             | ((((4660 as u32) as u16) as u32) as i32)
// CHECK-NEXT:                 << (2 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { us }) as u32) as i32) >> (4 as i32)
// CHECK-NEXT:         | (((unsafe { us }) as u32) as i32)
// CHECK-NEXT:             << (2 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != ((((4660 as u32) as u16) as u32) as i32) >> (4 as i32)
// CHECK-NEXT:             | ((((4660 as u32) as u16) as u32) as i32)
// CHECK-NEXT:                 << (2 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ui }) >> unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { ui })
// CHECK-NEXT:             << (4 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (4660 as u32) >> (4 as i32)
// CHECK-NEXT:             | (4660 as u32)
// CHECK-NEXT:                 << (4 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ui }) >> (4 as i32)
// CHECK-NEXT:         | (unsafe { ui })
// CHECK-NEXT:             << (4 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (4660 as u32) >> (4 as i32)
// CHECK-NEXT:             | (4660 as u32)
// CHECK-NEXT:                 << (4 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ul }) >> unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { ul })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (305419896 as u64) >> (4 as i32)
// CHECK-NEXT:             | (305419896 as u64)
// CHECK-NEXT:                 << (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ul }) >> (4 as i32)
// CHECK-NEXT:         | (unsafe { ul })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (305419896 as u64) >> (4 as i32)
// CHECK-NEXT:             | (305419896 as u64)
// CHECK-NEXT:                 << (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ull }) >> unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { ull })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (81985529234382576 as u64) >> (4 as i32)
// CHECK-NEXT:             | (81985529234382576 as u64)
// CHECK-NEXT:                 << (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ull }) >> (4 as i32)
// CHECK-NEXT:         | (unsafe { ull })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (81985529234382576 as u64) >> (4 as i32)
// CHECK-NEXT:             | (81985529234382576 as u64)
// CHECK-NEXT:                 << (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ull }) >> unsafe { shift2 }
// CHECK-NEXT:         | (unsafe { ull })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift2 }) as i64) as u64)
// CHECK-NEXT:         != (81985529234382576 as u64)
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:             | (81985529234382576 as u64)
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
// CHECK-NEXT:     if (unsafe { ull })
// CHECK-NEXT:         >> (8 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         | (unsafe { ull })
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(
// CHECK-NEXT:                     (8 as u64)
// CHECK-NEXT:                         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_sub(((4 as i32) as i64) as u64),
// CHECK-NEXT:                 )
// CHECK-NEXT:         != (81985529234382576 as u64)
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:             | (81985529234382576 as u64)
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
// CHECK-NEXT:     if (((unsafe { uc }) as u32) as i32) << unsafe { shift1 }
// CHECK-NEXT:         | (((unsafe { uc }) as u32) as i32)
// CHECK-NEXT:             >> (1 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != ((((4660 as u32) as u8) as u32) as i32) << (4 as i32)
// CHECK-NEXT:             | ((((4660 as u32) as u8) as u32) as i32)
// CHECK-NEXT:                 >> (1 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { uc }) as u32) as i32) << (4 as i32)
// CHECK-NEXT:         | (((unsafe { uc }) as u32) as i32)
// CHECK-NEXT:             >> (1 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != ((((4660 as u32) as u8) as u32) as i32) << (4 as i32)
// CHECK-NEXT:             | ((((4660 as u32) as u8) as u32) as i32)
// CHECK-NEXT:                 >> (1 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { us }) as u32) as i32) << unsafe { shift1 }
// CHECK-NEXT:         | (((unsafe { us }) as u32) as i32)
// CHECK-NEXT:             >> (2 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != ((((4660 as u32) as u16) as u32) as i32) << (4 as i32)
// CHECK-NEXT:             | ((((4660 as u32) as u16) as u32) as i32)
// CHECK-NEXT:                 >> (2 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { us }) as u32) as i32) << (4 as i32)
// CHECK-NEXT:         | (((unsafe { us }) as u32) as i32)
// CHECK-NEXT:             >> (2 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != ((((4660 as u32) as u16) as u32) as i32) << (4 as i32)
// CHECK-NEXT:             | ((((4660 as u32) as u16) as u32) as i32)
// CHECK-NEXT:                 >> (2 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ui }) << unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { ui })
// CHECK-NEXT:             >> (4 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (4660 as u32) << (4 as i32)
// CHECK-NEXT:             | (4660 as u32)
// CHECK-NEXT:                 >> (4 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ui }) << (4 as i32)
// CHECK-NEXT:         | (unsafe { ui })
// CHECK-NEXT:             >> (4 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (4660 as u32) << (4 as i32)
// CHECK-NEXT:             | (4660 as u32)
// CHECK-NEXT:                 >> (4 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ul }) << unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { ul })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (305419896 as u64) << (4 as i32)
// CHECK-NEXT:             | (305419896 as u64)
// CHECK-NEXT:                 >> (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ul }) << (4 as i32)
// CHECK-NEXT:         | (unsafe { ul })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (305419896 as u64) << (4 as i32)
// CHECK-NEXT:             | (305419896 as u64)
// CHECK-NEXT:                 >> (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ull }) << unsafe { shift1 }
// CHECK-NEXT:         | (unsafe { ull })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift1 }) as i64) as u64)
// CHECK-NEXT:         != (81985529234382576 as u64) << (4 as i32)
// CHECK-NEXT:             | (81985529234382576 as u64)
// CHECK-NEXT:                 >> (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ull }) << (4 as i32)
// CHECK-NEXT:         | (unsafe { ull })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         != (81985529234382576 as u64) << (4 as i32)
// CHECK-NEXT:             | (81985529234382576 as u64)
// CHECK-NEXT:                 >> (8 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ull }) << unsafe { shift2 }
// CHECK-NEXT:         | (unsafe { ull })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((unsafe { shift2 }) as i64) as u64)
// CHECK-NEXT:         != (81985529234382576 as u64)
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:             | (81985529234382576 as u64)
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
// CHECK-NEXT:     if (unsafe { ull })
// CHECK-NEXT:         << (8 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:         | (unsafe { ull })
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(
// CHECK-NEXT:                     (8 as u64)
// CHECK-NEXT:                         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_sub(((4 as i32) as i64) as u64),
// CHECK-NEXT:                 )
// CHECK-NEXT:         != (81985529234382576 as u64)
// CHECK-NEXT:             << (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((4 as i32) as i64) as u64)
// CHECK-NEXT:             | (81985529234382576 as u64)
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
