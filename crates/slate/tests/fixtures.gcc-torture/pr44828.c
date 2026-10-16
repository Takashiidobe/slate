extern void abort(void);

static signed char foo(signed char si1, signed char si2) { return si1 * si2; }

int a = 0x105F61CA;

int main(void) {
  int b = 0x0332F5C8;
  if (foo(b, a) > 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut si1: i8, mut si2: i8) -> i8 {
// CHECK-NEXT:     ((si1 as i32) * (si2 as i32)) as i8
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = 274686410 as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: i32 = 53671368 as i32;
// CHECK-NEXT:     if (foo(b as i8, (unsafe { a }) as i8) as i32) > (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
