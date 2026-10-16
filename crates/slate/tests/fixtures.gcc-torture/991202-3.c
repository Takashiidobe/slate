void abort(void);
void exit(int);

unsigned int f(unsigned int a) { return a * 65536 / 8; }

unsigned int g(unsigned int a) { return a * 65536; }

unsigned int h(unsigned int a) { return a / 8; }

int main() {
  if (f(65536) != h(g(65536)))
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
// CHECK-NEXT: fn f(mut a: u32) -> u32 {
// CHECK-NEXT:     a.wrapping_mul((65536 as i32) as u32) / ((8 as i32) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn g(mut a: u32) -> u32 {
// CHECK-NEXT:     a.wrapping_mul((65536 as i32) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn h(mut a: u32) -> u32 {
// CHECK-NEXT:     a / ((8 as i32) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f((65536 as i32) as u32) != h(g((65536 as i32) as u32)) {
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
