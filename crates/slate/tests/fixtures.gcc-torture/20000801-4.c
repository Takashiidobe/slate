/* Origin: PR c/128 from Martin Sebor <sebor@roguewave.com>, adapted
   as a testcase by Joseph Myers <jsm28@cam.ac.uk>.
*/
/* Character arrays initialized by a string literal must have
   uninitialized elements zeroed.  This isn't clear in the 1990
   standard, but was fixed in TC2 and C99; see DRs #060, #092.
*/
extern void abort(void);
extern void exit(int);

int foo(void) {
  char s[2] = "";
  return 0 == s[1];
}

char *t;

int main(void) {
  {
    char s[] = "x";
    t        = s;
  }
  if (foo())
    exit(0);
  else
    abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Origin: PR c/128 from Martin Sebor <sebor@roguewave.com>, adapted
// CHECK-NEXT: // as a testcase by Joseph Myers <jsm28@cam.ac.uk>.
// CHECK-NEXT: //
// CHECK-NEXT: // Character arrays initialized by a string literal must have
// CHECK-NEXT: // uninitialized elements zeroed.  This isn't clear in the 1990
// CHECK-NEXT: // standard, but was fixed in TC2 and C99; see DRs #060, #092.
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     let mut s: [i8; 2] = [0 as i8, 0 as i8];
// CHECK-NEXT:     ((0 as i32)
// CHECK-NEXT:         == ((unsafe { *unsafe { (s.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) } })
// CHECK-NEXT:             as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut t: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: [i8; 2] = [120 as i8, 0 as i8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         t = s.as_mut_ptr() as *mut i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo() != (0 as i32) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { abort() };
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
