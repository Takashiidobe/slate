void abort(void);
void exit(int);

void f(int *x) { *x = 0; }

int main(void) {
  int  s, c, x;
  char a[] = "c";

  f(&s);
  a[c = 0] = s == 0 ? (x = 1, 'a') : (x = 2, 'b');
  if (a[c] != 'a')
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
// CHECK-NEXT: fn f(mut x: &mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(x as *mut i32) = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0 as i32;
// CHECK-NEXT:     let mut c: i32 = 0 as i32;
// CHECK-NEXT:     let mut x: i32 = 0 as i32;
// CHECK-NEXT:     let mut a: [i8; 2] = [99 as i8, 0 as i8];
// CHECK-NEXT:     f(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32;
// CHECK-NEXT:     if s == (0 as i32) {
// CHECK-NEXT:         x = 1 as i32;
// CHECK-NEXT:         [[V_1]] = 97 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         x = 2 as i32;
// CHECK-NEXT:         [[V_1]] = 98 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) } = [[V_1]]
// CHECK-NEXT:             as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { *unsafe { (a.as_mut_ptr() as *mut i8).offset(c as isize) } }) as i32)
// CHECK-NEXT:         != (97 as i32)
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
