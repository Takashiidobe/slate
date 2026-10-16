void  abort(void);
void  exit(int);
char *doit(int flag) { return 1 + (flag ? "\0wrong\n" : "\0right\n"); }
int   main() {
  char *result = doit(0);
  if (*result == 'r' && result[1] == 'i')
    exit(0);
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn doit(mut flag: i32) -> *mut i8 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         if flag != (0 as i32) {
// CHECK-NEXT:             b"\0wrong\n\0".as_ptr() as *mut i8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             b"\0right\n\0".as_ptr() as *mut i8
// CHECK-NEXT:         }
// CHECK-NEXT:             .offset((1 as i32) as isize)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut result: *mut i8 = doit(0 as i32);
// CHECK-NEXT:     if ((unsafe { *result }) as i32) == (114 as i32)
// CHECK-NEXT:         && ((unsafe { *unsafe { result.offset((1 as i32) as isize) } }) as i32)
// CHECK-NEXT:             == (105 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
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
