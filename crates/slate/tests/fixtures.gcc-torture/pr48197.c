/* PR c/48197 */

extern void abort(void);
static int  y = 0x8000;

int main() {
  unsigned int x = (short)y;
  if (sizeof(0LL) == sizeof(0U))
    return 0;
  if (0LL > (0U ^ (short)-0x8000))
    abort();
  if (0LL > (0U ^ x))
    abort();
  if (0LL > (0U ^ (short)y))
    abort();
  if ((0U ^ (short)-0x8000) < 0LL)
    abort();
  if ((0U ^ x) < 0LL)
    abort();
  if ((0U ^ (short)y) < 0LL)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/48197
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut y: i32 = 32768 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u32 = (((unsafe { y }) as i16) as i32) as u32;
// CHECK-NEXT:     if (8 as u64) == (4 as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (0 as i64) > ((((0 as u32) ^ (((-(32768 as i32) as i16) as i32) as u32)) as u64) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (0 as i64) > ((((0 as u32) ^ x) as u64) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (0 as i64) > ((((0 as u32) ^ ((((unsafe { y }) as i16) as i32) as u32)) as u64) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((((0 as u32) ^ (((-(32768 as i32) as i16) as i32) as u32)) as u64) as i64) < (0 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((((0 as u32) ^ x) as u64) as i64) < (0 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((((0 as u32) ^ ((((unsafe { y }) as i16) as i32) as u32)) as u64) as i64) < (0 as i64) {
// CHECK-NEXT:         unsafe { abort() };
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
