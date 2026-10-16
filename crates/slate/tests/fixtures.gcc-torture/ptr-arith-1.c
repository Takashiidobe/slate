void abort(void);
void exit(int);

char *f(char *s, unsigned int i) { return &s[i + 3 - 1]; }

int main(void) {
  char *str = "abcdefghijkl";
  char *x2  = f(str, 12);
  if (str + 14 != x2)
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
// CHECK-NEXT: fn f(mut s: *mut i8, mut i: u32) -> *mut i8 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.offset(
// CHECK-NEXT:             i.wrapping_add((3 as i32) as u32).wrapping_sub((1 as i32) as u32) as isize,
// CHECK-NEXT:         )
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut str: *mut i8 = c"abcdefghijkl".as_ptr() as *mut i8;
// CHECK-NEXT:     let mut x2: *mut i8 = f(str, (12 as i32) as u32);
// CHECK-NEXT:     if (unsafe { str.offset((14 as i32) as isize) }) != x2 {
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
