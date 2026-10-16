void abort(void);
void exit(int);
int  l[] = {0, 1};
int  main(void) {
  int *p = l;
  switch (*p++) {
  case 0:
    exit(0);
  case 1:
    break;
  case 2:
    break;
  case 3:
  case 4:
    break;
  }
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut l: [i32; 2] = [0 as i32, 1 as i32];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { std::ptr::addr_of_mut!(l) as *mut i32 };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut i32 = unsafe { [[V_1]].offset((1 as i32) as isize) };
// CHECK-NEXT:     p = [[V_2]];
// CHECK-NEXT:     match unsafe { *[[V_1]] } {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             unsafe { exit(0 as i32) };
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {}
// CHECK-NEXT:         2 => {}
// CHECK-NEXT:         3 | 4 => {}
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { abort() };
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
