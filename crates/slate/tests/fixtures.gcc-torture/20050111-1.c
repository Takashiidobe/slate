/* PR middle-end/19084, rtl-optimization/19348 */

unsigned int foo(unsigned long long x) {
  unsigned int u;

  if (x == 0)
    return 0;
  u = (unsigned int)(x >> 32);
  return u;
}

unsigned long long bar(unsigned short x) { return (unsigned long long)x << 32; }

extern void abort(void);

int main(void) {
  if (sizeof(long long) != 8)
    return 0;

  if (foo(0) != 0)
    abort();
  if (foo(0xffffffffULL) != 0)
    abort();
  if (foo(0x25ff00ff00ULL) != 0x25)
    abort();
  if (bar(0) != 0)
    abort();
  if (bar(0x25) != 0x2500000000ULL)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/19084, rtl-optimization/19348
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u64) -> u32 {
// CHECK-NEXT:     let mut u: u32 = 0 as u32;
// CHECK-NEXT:     if x == (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         return (0 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     u = (x >> (32 as i32)) as u32;
// CHECK-NEXT:     u
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: u16) -> u64 {
// CHECK-NEXT:     (x as u64) << (32 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as u64) != (((8 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(((0 as i32) as i64) as u64) != ((0 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(4294967295 as u64) != ((0 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(163192045312 as u64) != ((37 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(((0 as i32) as i16) as u16) != (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(((37 as i32) as i16) as u16) != (158913789952 as u64) {
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
