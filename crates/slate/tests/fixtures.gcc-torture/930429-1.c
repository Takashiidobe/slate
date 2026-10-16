void abort(void);
void exit(int);

char *f(char *p) {
  short x = *p++ << 16;
  return p;
}

int main(void) {
  char *p = "";
  if (f(p) != p + 1)
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
// CHECK-NEXT: fn f(mut p: *mut i8) -> *mut i8 {
// CHECK-NEXT:     let mut x: i16 = 0 as i16;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut i8 = unsafe { [[V_1]].offset((1 as i32) as isize) };
// CHECK-NEXT:     p = [[V_2]];
// CHECK-NEXT:     x = (((unsafe { *[[V_1]] }) as i32) << (16 as i32)) as i16;
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: *mut i8 = c"".as_ptr() as *mut i8;
// CHECK-NEXT:     if f(p) != unsafe { p.offset((1 as i32) as isize) } {
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
