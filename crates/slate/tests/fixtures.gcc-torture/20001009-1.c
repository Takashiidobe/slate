void abort(void);
int  a, b;
int  main(void) {
  int c = -2;
  int d = 0xfe;
  int e = a & 1;
  int f = b & 2;
  if ((char)(c | (e & f)) == (char)d)
    return 0;
  else
    abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c: i32 = -(2 as i32);
// CHECK-NEXT:     let mut d: i32 = 254 as i32;
// CHECK-NEXT:     let mut e: i32 = (unsafe { a }) & (1 as i32);
// CHECK-NEXT:     let mut f: i32 = (unsafe { b }) & (2 as i32);
// CHECK-NEXT:     if (((c | e & f) as i8) as i32) == ((d as i8) as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
