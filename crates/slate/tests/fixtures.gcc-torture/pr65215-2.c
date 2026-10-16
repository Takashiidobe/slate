/* PR tree-optimization/65215 */

static inline unsigned int foo(unsigned int x) {
  return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << 24);
}

__attribute__((noinline, noclone)) unsigned long long
bar(unsigned long long *x) {
  return ((unsigned long long)foo(*x) << 32) | foo(*x >> 32);
}

int main() {
  if (__CHAR_BIT__ != 8 || sizeof(unsigned int) != 4 ||
      sizeof(unsigned long long) != 8)
    return 0;
  unsigned long long l =
      foo(0xfeedbea8U) | ((unsigned long long)foo(0xdeadbeefU) << 32);
  if (bar(&l) != 0xfeedbea8deadbeefULL)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/65215
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32) -> u32 {
// CHECK-NEXT:     x >> (24 as i32)
// CHECK-NEXT:         | x >> (8 as i32) & ((65280 as i32) as u32)
// CHECK-NEXT:         | x << (8 as i32) & ((16711680 as i32) as u32)
// CHECK-NEXT:         | x << (24 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: &u64) -> u64 {
// CHECK-NEXT:     (foo((unsafe { *(x as *const u64) }) as u32) as u64) << (32 as i32)
// CHECK-NEXT:         | (foo(((unsafe { *(x as *const u64) }) >> (32 as i32)) as u32) as u64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as i32) != (8 as i32)
// CHECK-NEXT:         || (4 as u64) != (((4 as i32) as i64) as u64)
// CHECK-NEXT:         || (8 as u64) != (((8 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut l: u64 =
// CHECK-NEXT:         (foo(4276993704 as u32) as u64) | (foo(3735928559 as u32) as u64) << (32 as i32);
// CHECK-NEXT:     if bar(unsafe { &(*std::ptr::addr_of_mut!(l)) }) != (18369548087613832943 as u64) {
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
