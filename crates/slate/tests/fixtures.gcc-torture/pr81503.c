unsigned short a = 41461;
unsigned short b = 3419;
#if __SIZEOF_INT__ >= 4
int c = 0;

void foo() {
  if (a + b * ~(0 != 5))
    c = -~(b * ~(0 != 5)) + 2147483647;
}
#else
__INT32_TYPE__ c = 0;

void foo() {
  if (a + b * ~((__INT32_TYPE__)(0 != 5)))
    c = -~(b * ~((__INT32_TYPE__)(0 != 5))) + 2147483647;
}
#endif

int main() {
  foo();
  if (c != 2147476810)
    return -1;
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: static mut a: u16 = ((41461 as i32) as i16) as u16;
// CHECK-NEXT: static mut b: u16 = ((3419 as i32) as i16) as u16;
// CHECK-NEXT: static mut c: i32 = 0 as i32;
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     if (((unsafe { a }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe { b }) as u32) as i32) * !(((0 as i32) != (5 as i32)) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = -(!((((unsafe { b }) as u32) as i32)
// CHECK-NEXT:                 * !(((0 as i32) != (5 as i32)) as i32))) + (2147483647 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
// CHECK-NEXT:     if (unsafe { c }) != (2147476810 as i32) {
// CHECK-NEXT:         return -(1 as i32);
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
