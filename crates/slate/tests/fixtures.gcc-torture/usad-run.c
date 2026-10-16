extern void abort();
extern int  abs(int __x) __attribute__((__nothrow__, __leaf__))
__attribute__((__const__));

static int foo(unsigned char *w, int i, unsigned char *x, int j) {
  int tot = 0;
  for (int a = 0; a < 16; a++) {
    for (int b = 0; b < 16; b++)
      tot += abs(w[b] - x[b]);
    w += i;
    x += j;
  }
  return tot;
}

void bar(unsigned char *w, unsigned char *x, int i, int *result) {
  *result = foo(w, 16, x, i);
}

int main(void) {
  unsigned char m[256];
  unsigned char n[256];
  int           sum, i;

  for (i = 0; i < 256; ++i)
    if (i % 2 == 0) {
      m[i] = (i % 8) * 2 + 1;
      n[i] = -(i % 8);
    } else {
      m[i] = -((i % 8) * 2 + 2);
      n[i] = -((i % 8) >> 1);
    }

  bar(m, n, 16, &sum);

  if (sum != 32384)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn abs(__x: i32) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn foo(mut w: *mut u8, mut i: i32, mut x: *mut u8, mut j: i32) -> i32 {
// CHECK-NEXT:     let mut tot: i32 = 0 as i32;
// CHECK-NEXT:     let mut a: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while a < (16 as i32) {
// CHECK-NEXT:         let mut b: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while b < (16 as i32) {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = tot;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]]
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     abs(
// CHECK-NEXT:                         (((unsafe { *unsafe { w.offset(b as isize) } }) as u32) as i32)
// CHECK-NEXT:                             - (((unsafe { *unsafe { x.offset(b as isize) } }) as u32)
// CHECK-NEXT:                                 as i32),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 };
// CHECK-NEXT:             tot = [[V_1]];
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             b = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: *mut u8 = w;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: *mut u8 = unsafe { [[V_4]].offset(i as isize) };
// CHECK-NEXT:         w = [[V_5]];
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: *mut u8 = x;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: *mut u8 = unsafe { [[V_6]].offset(j as isize) };
// CHECK-NEXT:         x = [[V_7]];
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:         a = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     tot
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut w: *mut u8, mut x: *mut u8, mut i: i32, mut result: &mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(result as *mut i32) = foo(w, 16 as i32, x, i);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut m: __SlateAlign16<[u8; 256]> = __SlateAlign16([0 as u8; 256]);
// CHECK-NEXT:     let mut n: __SlateAlign16<[u8; 256]> = __SlateAlign16([0 as u8; 256]);
// CHECK-NEXT:     let mut sum: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (256 as i32) {
// CHECK-NEXT:         if i % (2 as i32) == (0 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (m.0.as_mut_ptr() as *mut u8).offset(i as isize) } = ((i
// CHECK-NEXT:                     % (8 as i32) * (2 as i32) + (1 as i32)) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (n.0.as_mut_ptr() as *mut u8).offset(i as isize) } = (-(i
// CHECK-NEXT:                     % (8 as i32)) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (m.0.as_mut_ptr() as *mut u8).offset(i as isize) } = (-(i
// CHECK-NEXT:                     % (8 as i32) * (2 as i32) + (2 as i32)) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (n.0.as_mut_ptr() as *mut u8).offset(i as isize) } = (-(i
// CHECK-NEXT:                     % (8 as i32) >> (1 as i32)) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] + (1 as i32);
// CHECK-NEXT:         i = [[V_11]];
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(
// CHECK-NEXT:         m.0.as_mut_ptr() as *mut u8,
// CHECK-NEXT:         n.0.as_mut_ptr() as *mut u8,
// CHECK-NEXT:         16 as i32,
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(sum)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     if sum != (32384 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
