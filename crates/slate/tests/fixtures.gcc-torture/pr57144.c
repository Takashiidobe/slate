extern void abort(void);
extern void exit(int);

void __attribute__((noinline)) foo(int a) {
  int       z = a > 0 ? a : -a;
  long long x = z;
  if (x > 0x100000000LL)
    abort();
  else
    exit(0);
}

int main() { foo(1); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut a: i32) {
// CHECK-NEXT:     let mut z: i32 = if a > (0 as i32) { a } else { -a };
// CHECK-NEXT:     let mut x: i64 = z as i64;
// CHECK-NEXT:     if x > (4294967296 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(1 as i32);
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
