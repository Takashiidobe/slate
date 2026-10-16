/* PR tree-optimization/98736 */

int  a[6];
char b, c;
int  main() {
  int d[4] = {0, 0, 0, 0};
  for (c = 0; c <= 5; c++) {
    for (b = 2; b != 0; b++)
      a[c] = 8;
    a[c] = d[3];
  }
  if (a[0] != 0)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/98736
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: __SlateAlign16<[i32; 6]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: __SlateAlign16<[i32; 4]> = __SlateAlign16([0 as i32, 0 as i32, 0 as i32, 0 as i32]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((unsafe { c }) as i32) <= (5 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = (2 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while ((unsafe { b }) as i32) != (0 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                         .offset(((unsafe { c }) as i32) as isize)
// CHECK-NEXT:                 } = 8 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i8 = unsafe { b };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i8 = (([[V_0]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 b = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                     .offset(((unsafe { c }) as i32) as isize)
// CHECK-NEXT:             } = unsafe { *unsafe { (d.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i8 = unsafe { c };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i8 = (([[V_2]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }.offset((0 as i32) as isize) }
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
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
