#include <stdio.h>

void abort(void);
void exit(int);

int g(void) { return '\n'; }

void f(void) {
  char  s[] = "abcedfg012345";
  char *sp  = s + 12;

  switch (g()) {
  case '\n':
    break;
  }

  while (*--sp == '0')
    ;
  sprintf(sp + 1, "X");

  if (s[12] != 'X')
    abort();
}

int main(void) {
  f();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn sprintf(__s: *mut i8, __format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn g() -> i32 {
// CHECK-NEXT:     10 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn f() {
// CHECK-NEXT:     let mut s: [i8; 14] = [
// CHECK-NEXT:         97 as i8,
// CHECK-NEXT:         98 as i8,
// CHECK-NEXT:         99 as i8,
// CHECK-NEXT:         101 as i8,
// CHECK-NEXT:         100 as i8,
// CHECK-NEXT:         102 as i8,
// CHECK-NEXT:         103 as i8,
// CHECK-NEXT:         48 as i8,
// CHECK-NEXT:         49 as i8,
// CHECK-NEXT:         50 as i8,
// CHECK-NEXT:         51 as i8,
// CHECK-NEXT:         52 as i8,
// CHECK-NEXT:         53 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut sp: *mut i8 = unsafe {
// CHECK-NEXT:         (s.as_mut_ptr() as *mut i8).offset((12 as i32) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match g() {
// CHECK-NEXT:             10 => {}
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut i8 = sp;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut i8 = unsafe { [[V_1]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         sp = [[V_2]];
// CHECK-NEXT:         if !(((unsafe { *[[V_2]] }) as i32) == (48 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         {}
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sprintf(unsafe { sp.offset((1 as i32) as isize) }, c"X".as_ptr() as *const i8)
// CHECK-NEXT:     };
// CHECK-NEXT:     if ((unsafe { *unsafe { (s.as_mut_ptr() as *mut i8).offset((12 as i32) as isize) } })
// CHECK-NEXT:         as i32) != (88 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f();
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
