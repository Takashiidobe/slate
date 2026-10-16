
unsigned int a = 1387579096U;
void         sinkandcheck(unsigned b) __attribute__((noipa));
void         sinkandcheck(unsigned b) {
  if (a != b)
    __builtin_abort();
}
int main() {
  a = 1 < (~a) ? 1 : (~a);
  sinkandcheck(1);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: u32 = 1387579096 as u32;
// CHECK-NEXT: fn sinkandcheck(mut b: u32) {
// CHECK-NEXT:     if (unsafe { a }) != b {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = if ((1 as i32) as u32) < !unsafe { a } {
// CHECK-NEXT:             (1 as i32) as u32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             !unsafe { a }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     sinkandcheck((1 as i32) as u32);
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
