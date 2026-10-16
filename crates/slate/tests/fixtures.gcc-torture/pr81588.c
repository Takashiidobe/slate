/* PR tree-optimization/81588 */

__attribute__((noinline, noclone)) int bar(int x) {
  __asm volatile("" : : "g"(x) : "memory");
}

__attribute__((noinline, noclone)) int foo(unsigned x, long long y) {
  if (y < 0)
    return 0;
  if (y < (long long)(4 * x)) {
    bar(y);
    return 1;
  }
  return 0;
}

int main() {
  volatile unsigned  x = 10;
  volatile long long y = -10000;
  if (foo(x, y) != 0)
    __builtin_abort();
  y = -1;
  if (foo(x, y) != 0)
    __builtin_abort();
  y = 0;
  if (foo(x, y) != 1)
    __builtin_abort();
  y = 39;
  if (foo(x, y) != 1)
    __builtin_abort();
  y = 40;
  if (foo(x, y) != 0)
    __builtin_abort();
  y = 10000;
  if (foo(x, y) != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/81588
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("\n# {0:e}", in(reg) x as u32, options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32, mut y: i64) -> i32 {
// CHECK-NEXT:     if y < ((0 as i32) as i64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if y < ((((4 as i32) as u32).wrapping_mul(x) as u64) as i64) {
// CHECK-NEXT:         bar(y as i32);
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u32 = (10 as i32) as u32;
// CHECK-NEXT:     let mut y: i64 = -(10000 as i32) as i64;
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) },
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) },
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), -(1 as i32) as i64) };
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) },
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) },
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), (0 as i32) as i64) };
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) },
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) },
// CHECK-NEXT:     ) != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), (39 as i32) as i64) };
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) },
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) },
// CHECK-NEXT:     ) != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), (40 as i32) as i64) };
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) },
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) },
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), (10000 as i32) as i64) };
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) },
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) },
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
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
