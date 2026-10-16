/* { dg-require-effective-target int32plus } */
int __attribute__((noinline)) foo(void) { return 123; }

int __attribute__((noinline)) bar(void) {
  int c  = 1;
  c     |= 4294967295 ^ (foo() | 4073709551608);
  return c;
}

int main() {
  if (bar() != 0x83fd4005)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// { dg-require-effective-target int32plus }
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     123 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> i32 {
// CHECK-NEXT:     let mut c: i32 = 1 as i32;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = c;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 =
// CHECK-NEXT:         (([[V_0]] as i64) | (4294967295 as i64) ^ ((foo() as i64) | (4073709551608 as i64))) as i32;
// CHECK-NEXT:     c = [[V_1]];
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (bar() as u32) != (2214412293 as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
