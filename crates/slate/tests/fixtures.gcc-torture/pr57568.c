/* PR target/57568 */

extern void abort(void);
int         a[6][9] = {}, b = 1, *c = &a[3][5];

int main() {
  if (b && (*c = *c + *c))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/57568
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: __SlateAlign16<{{\[\[}}i32; 9]; 6]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: {{\[\[}}i32; 9]; 6] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: *mut i32 = unsafe {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut [i32; 9] }.offset((3 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:         .as_mut_ptr() as *mut i32
// CHECK-NEXT:     }
// CHECK-NEXT:     .offset((5 as i32) as isize)
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { b }) != (0 as i32) {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = (unsafe { *unsafe { c } }) + unsafe { *unsafe { c } };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { c } = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_0]] = [[V_1]] != (0 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
