static int __attribute__((noinline, noclone)) swap(int x) {
  return (unsigned short)((unsigned short)x << 8 | (unsigned short)x >> 8);
}

static int a = 0x1234;

int main(void) {
  int b = 0x1234;
  if (swap(a) != 0x3412)
    __builtin_abort();
  if (swap(b) != 0x3412)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn swap(mut x: i32) -> i32 {
// CHECK-NEXT:     ((((((((x as i16) as u16) as u32) as i32) << (8 as i32)
// CHECK-NEXT:         | ((((x as i16) as u16) as u32) as i32) >> (8 as i32)) as i16) as u16) as u32)
// CHECK-NEXT:         as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = 4660 as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: i32 = 4660 as i32;
// CHECK-NEXT:     if swap(unsafe { a }) != (13330 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if swap(b) != (13330 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
