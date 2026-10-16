void abort(void);
void exit(int);

int w[2][2];

void f(void) {
  int i, j;

  for (i = 0; i < 2; i++)
    for (j = 0; j < 2; j++)
      if (i == j)
        w[i][j] = 1;
}

int main(void) {
  f();
  if (w[0][0] != 1 || w[1][1] != 1 || w[1][0] != 0 || w[0][1] != 0)
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut w: __SlateAlign16<{{\[\[}}i32; 2]; 2]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn f() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         j = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < (2 as i32) {
// CHECK-NEXT:             if i == j {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of_mut!(w.0) as *mut [i32; 2] }
// CHECK-NEXT:                                     .offset(i as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                                 .as_mut_ptr() as *mut i32
// CHECK-NEXT:                         }
// CHECK-NEXT:                             .offset(j as isize)
// CHECK-NEXT:                     } = 1 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:             j = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f();
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(w.0) as *mut [i32; 2] }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .as_mut_ptr() as *mut i32
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (1 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(w.0) as *mut [i32; 2] }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut i32
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (1 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(w.0) as *mut [i32; 2] }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut i32
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(w.0) as *mut [i32; 2] }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut i32
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (0 as i32)
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
