long long a;
unsigned  b;
int       c[70];
int       d[70][70];
int       e;

__attribute__((noinline)) void f(long long *g, int p2) { *g = p2; }

__attribute__((noinline)) void fn2() {
  for (int j = 0; j < 70; j++) {
    for (int i = 0; i < 70; i++) {
      if (b)
        c[i] = 0;
      for (int l = 0; l < 70; l++)
        d[i][1] = d[l][i];
    }
    for (int k = 0; k < 70; k++)
      e = c[0];
  }
}

int main() {
  b = 5;
  for (int j = 0; j < 70; ++j)
    c[j] = 2075593088;
  fn2();
  f(&a, e);
  if (a)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut a: i64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: __SlateAlign16<[i32; 70]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT: static mut d: __SlateAlign16<{{\[\[}}i32; 70]; 70]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn f(mut g: *mut i64, mut p2: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *g = p2 as i64;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn fn2() {
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while j < (70 as i32) {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (70 as i32) {
// CHECK-NEXT:             if (unsafe { b }) != (0 as u32) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(c.0) as *mut i32 }
// CHECK-NEXT:                             .offset(i as isize)
// CHECK-NEXT:                     } = 0 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut l: i32 = 0 as i32;
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while l < (70 as i32) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of_mut!(d.0) as *mut [i32; 70] }
// CHECK-NEXT:                                     .offset(i as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                                 .as_mut_ptr() as *mut i32
// CHECK-NEXT:                         }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                     } = unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of_mut!(d.0) as *mut [i32; 70] }
// CHECK-NEXT:                                         .offset(l as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                     .as_mut_ptr() as *mut i32
// CHECK-NEXT:                             }
// CHECK-NEXT:                                 .offset(i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_0:__v[0-9]+]]: i32 = l;
// CHECK-NEXT:                 let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:                 l = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             i = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut k: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while k < (70 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 e = unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(c.0) as *mut i32 }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:             k = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         j = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = (5 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while j < (70 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(c.0) as *mut i32 }.offset(j as isize)
// CHECK-NEXT:             } = 2075593088 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:         j = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2();
// CHECK-NEXT:     f(unsafe { std::ptr::addr_of_mut!(a) }, unsafe { e });
// CHECK-NEXT:     if (unsafe { a }) != (0 as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
