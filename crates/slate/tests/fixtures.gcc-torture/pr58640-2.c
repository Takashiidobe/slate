extern void abort(void);

int a[20], b, c;

int fn1() {
  int d, e, f, g = 0;

  a[12] = 1;
  for (e = 0; e < 3; e++)
    for (d = 0; d < 2; d++) {
      for (f = 0; f < 2; f++) {
        g ^= a[12] > 1;
        if (g)
          return 0;
        if (b)
          break;
      }
      for (c = 0; c < 1; c++)
        a[d] = a[e * 3 + 9];
    }
  return 0;
}

int main() {
  fn1();
  if (a[0] != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut a: __SlateAlign16<[i32; 20]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn fn1() -> i32 {
// CHECK-NEXT:     let mut d: i32 = 0 as i32;
// CHECK-NEXT:     let mut e: i32 = 0 as i32;
// CHECK-NEXT:     let mut f: i32 = 0 as i32;
// CHECK-NEXT:     let mut g: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                 .offset((12 as i32) as isize)
// CHECK-NEXT:         } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while e < (3 as i32) {
// CHECK-NEXT:         d = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while d < (2 as i32) {
// CHECK-NEXT:             f = 0 as i32;
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while f < (2 as i32) {
// CHECK-NEXT:                 let mut [[V_0:__v[0-9]+]]: i32 = g;
// CHECK-NEXT:                 let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]]
// CHECK-NEXT:                     ^ (((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                                 .offset((12 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) > (1 as i32)) as i32);
// CHECK-NEXT:                 g = [[V_1]];
// CHECK-NEXT:                 if g != (0 as i32) {
// CHECK-NEXT:                     return 0 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if (unsafe { b }) != (0 as i32) {
// CHECK-NEXT:                     break '[[SLATE_BREAK_2]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_2:__v[0-9]+]]: i32 = f;
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:                 f = [[V_3]];
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 c = 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while (unsafe { c }) < (1 as i32) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                             .offset(d as isize)
// CHECK-NEXT:                     } = unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                                 .offset((e * (3 as i32) + (9 as i32)) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     c = [[V_5]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:             d = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = e;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:         e = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     fn1();
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
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
