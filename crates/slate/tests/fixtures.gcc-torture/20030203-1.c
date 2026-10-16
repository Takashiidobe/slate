void f(int);
int  do_layer3(int single) {
  int stereo1;

  if (single >= 0) /* stream is stereo, but force to mono */
    stereo1 = 1;
  else
    stereo1 = 2;
  f(single);

  return stereo1;
}

extern void abort();
int         main() {
  if (do_layer3(-1) != 2)
    abort();
  return 0;
}

void f(int i) {}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn do_layer3(mut single: i32) -> i32 {
// CHECK-NEXT:     let mut stereo1: i32 = 0 as i32;
// CHECK-NEXT:     if single >= (0 as i32) {
// CHECK-NEXT:         stereo1 = 1 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         stereo1 = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     // stream is stereo, but force to mono
// CHECK-NEXT:     f(single);
// CHECK-NEXT:     stereo1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if do_layer3(-(1 as i32)) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut i: i32) {}
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
