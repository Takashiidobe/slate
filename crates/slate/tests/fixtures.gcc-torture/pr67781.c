/* { dg-require-effective-target int32plus } */
#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#else
typedef unsigned uint32_t;
#endif

#ifdef __UINT8_TYPE__
typedef __UINT8_TYPE__ uint8_t;
#else
typedef unsigned char uint8_t;
#endif

struct {
  uint32_t a;
  uint8_t  b;
} s = {0x123456, 0x78};

int pr67781() {
  uint32_t c = (s.a << 8) | s.b;
  return c;
}

int main() {
  if (sizeof(uint32_t) * __CHAR_BIT__ != 32)
    return 0;

  if (pr67781() != 0x12345678)
    __builtin_abort();
  return 0;
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: u32,
// CHECK-NEXT:     b: u8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:     a: (1193046 as i32) as u32,
// CHECK-NEXT:     b: ((120 as i32) as i8) as u8,
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn pr67781() -> i32 {
// CHECK-NEXT:     let mut c: u32 = (unsafe { s.a }) << (8 as i32) | ((((unsafe { s.b }) as u32) as i32) as u32);
// CHECK-NEXT:     c as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) != (((32 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if pr67781() != (305419896 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
