#include <stdio.h>
#include <stddef.h>

static int probe(const char *p) { return p ? (int)p[0] : 0; }

int both(const char *p) {
  return p != NULL && probe(p) != 0;
}

int either(const char *p, int a) {
  int r;
  if (p != NULL) {
    int t = probe(p);
    r = t + a;
  } else {
    r = -1;
  }
  return r;
}

int zeroed(int a) {
  int r = 0;
  if (a > 3) {
    r = a * 2;
  } else {
    r = a + 7;
  }
  return r + 1;
}

int one_armed(int a) {
  int r;
  if (a > 3) {
    r = a;
  } else {
    printf("small\n");
    r = 0;
    r += 2;
  }
  return r;
}

int reads_before(int a) {
  int r = 5;
  if (a > 3) {
    r = r + a;
  } else {
    r = 1;
  }
  return r;
}

int main(void) {
  printf("%d %d %d\n", both("x"), both(NULL), either("a", 2));
  printf("%d %d %d %d\n", zeroed(1), zeroed(9), one_armed(1), one_armed(8));
  printf("%d %d\n", reads_before(1), reads_before(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn probe(p: *const i8) -> i32 {
// CHECK-NEXT:     if !p.is_null() { (unsafe { *p.add(0) }) as i32 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn both(p: *const i8) -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = !p.is_null() && probe(p) != 0;
// CHECK-NEXT:     [[V_0]] as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn either(p: *const i8, a: i32) -> i32 {
// CHECK-NEXT:     let r: i32 = if !p.is_null() {
// CHECK-NEXT:         let t: i32 = probe(p);
// CHECK-NEXT:         t + a
// CHECK-NEXT:     } else {
// CHECK-NEXT:         -1
// CHECK-NEXT:     };
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn zeroed(a: i32) -> i32 {
// CHECK-NEXT:     let r: i32 = if a > 3 { a * 2 } else { a + 7 };
// CHECK-NEXT:     r + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn one_armed(a: i32) -> i32 {
// CHECK-NEXT:     let mut r: i32 = 0;
// CHECK-NEXT:     if a > 3 {
// CHECK-NEXT:         r = a;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("small");
// CHECK-NEXT:         r = 0;
// CHECK-NEXT:         r += 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn reads_before(a: i32) -> i32 {
// CHECK-NEXT:     let mut r: i32 = 5;
// CHECK-NEXT:     if a > 3 {
// CHECK-NEXT:         r += a;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         r = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", both(c"x".as_ptr()), both(std::ptr::null::< i8 > ()), either(c"a"
// CHECK-NEXT:         .as_ptr(), 2i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", zeroed(1i32), zeroed(9i32), one_armed(1i32), one_armed(8i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", reads_before(1i32), reads_before(9i32));
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
