int a = 0x101;
int b = 0x100;

int test(void) {
  return (((unsigned char)(unsigned long long)((a ? a : 1) & (a * b))) ? 0 : 1);
}

int main(void) { return 1 - test(); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: static mut a: i32 = 257 as i32;
// CHECK-NEXT: static mut b: i32 = 256 as i32;
// CHECK-NEXT: fn test() -> i32 {
// CHECK-NEXT:     if (((((if (unsafe { a }) != (0 as i32) { unsafe { a } } else { 1 as i32 })
// CHECK-NEXT:         & (unsafe { a }) * unsafe { b }) as i64) as u64) as u8) != (0 as u8)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         1 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     (1 as i32) - test()
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
