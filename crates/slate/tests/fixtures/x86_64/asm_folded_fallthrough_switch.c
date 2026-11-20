#include <stdio.h>

extern void __bad_size(void);

#define STEP(x) x = x * 3 + 1; x ^= x >> 3;
#define TAIL(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x)

#define WIDEN(x, p)                                                            \
  do {                                                                         \
    switch (sizeof(*(p))) {                                                    \
    case 1:                                                                    \
      __asm__("movzbl %1, %k0" : "=r"(x) : "q"(*(p)));                         \
    case 2:                                                                    \
      TAIL(x)                                                                  \
      break;                                                                   \
    case 4:                                                                    \
      __asm__("movl %1, %k0" : "=r"(x) : "r"(*(p)));                           \
    case 8:                                                                    \
      TAIL(x)                                                                  \
      break;                                                                   \
    default:                                                                   \
      __bad_size();                                                            \
    }                                                                          \
  } while (0)

// @check-fn-begin
static unsigned long widen_u8(unsigned char *p) { unsigned long x = 0; WIDEN(x, p); return x; }
// @check-fn-end
static unsigned long widen_u32(unsigned int *p) { unsigned long x = 0; WIDEN(x, p); return x; }
static unsigned long widen_u64(unsigned long *p) { unsigned long x = 5; WIDEN(x, p); return x; }

int main(void) {
  unsigned char a = 3;
  unsigned int b = 4;
  unsigned long c = 6;
  printf("%lu %lu %lu\n", widen_u8(&a), widen_u32(&b), widen_u64(&c));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn widen_u8(mut p: &u8) -> u64 {
// CHECK-NEXT:     let mut x: u64 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: i64 = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match [[T_0]] {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         let [[T_1:__t[0-9]+]]: u64;
// CHECK-NEXT:                         core::arch::asm!(
// CHECK-NEXT:                             "movzbl {1:l}, {0:e}",
// CHECK-NEXT:                             lateout(reg) [[T_1]],
// CHECK-NEXT:                             in(reg) (*(p as *const u8) as u8) as u32,
// CHECK-NEXT:                             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         x = unsafe { core::mem::transmute::<u64, u64>([[T_1]] as u64) };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     [[T_0]] = 1;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     x = x.wrapping_mul(3).wrapping_add(1);
// CHECK-NEXT:                     x ^= x >> 3i32;
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     STEP!(x);
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// SLATE-FILECHECK-END
