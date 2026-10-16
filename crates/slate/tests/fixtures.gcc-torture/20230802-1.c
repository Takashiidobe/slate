/*  We used to simplify these incorrectly.  */
__attribute__((noipa)) long long foo(unsigned int x) {
  int y = x;
  y     = ~y;
  return ((long long)x) & y;
}

__attribute__((noipa)) long long foo_v(volatile unsigned int x) {
  volatile int y = x;
  y              = ~y;
  return ((long long)x) & y;
}

__attribute__((noipa)) long long bar(unsigned int x) {
  int y = x;
  y     = ~y;
  return ((long long)x) ^ y;
}

__attribute__((noipa)) long long bar_v(volatile unsigned int x) {
  volatile int y = x;
  y              = ~y;
  return ((long long)x) ^ y;
}

__attribute__((noipa)) long long baz(unsigned int x) {
  int y = x;
  y     = ~y;
  return y ^ ((long long)x);
}

__attribute__((noipa)) long long baz_v(volatile unsigned int x) {
  volatile int y = x;
  y              = ~y;
  return y ^ ((long long)x);
}

int main() {
  for (int t = -1; t <= 1; t++) {
    if (foo(t) != foo_v(t))
      __builtin_abort();
    if (bar(t) != bar_v(t))
      __builtin_abort();
    if (baz(t) != baz_v(t))
      __builtin_abort();
  }
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
// CHECK-NEXT: ///  We used to simplify these incorrectly.
// CHECK-NEXT: fn foo(mut x: u32) -> i64 {
// CHECK-NEXT:     let mut y: i32 = x as i32;
// CHECK-NEXT:     y = !y;
// CHECK-NEXT:     ((x as u64) as i64) & (y as i64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo_v(mut x: u32) -> i64 {
// CHECK-NEXT:     let mut y: i32 = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(y), !unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(y))
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as u64) as i64)
// CHECK-NEXT:         & ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: u32) -> i64 {
// CHECK-NEXT:     let mut y: i32 = x as i32;
// CHECK-NEXT:     y = !y;
// CHECK-NEXT:     ((x as u64) as i64) ^ (y as i64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar_v(mut x: u32) -> i64 {
// CHECK-NEXT:     let mut y: i32 = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(y), !unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(y))
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as u64) as i64)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut x: u32) -> i64 {
// CHECK-NEXT:     let mut y: i32 = x as i32;
// CHECK-NEXT:     y = !y;
// CHECK-NEXT:     (y as i64) ^ ((x as u64) as i64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz_v(mut x: u32) -> i64 {
// CHECK-NEXT:     let mut y: i32 = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(y), !unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(y))
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i64)
// CHECK-NEXT:         ^ (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as u64) as i64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut t: i32 = -(1 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while t <= (1 as i32) {
// CHECK-NEXT:         if foo(t as u32) != foo_v(t as u32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if bar(t as u32) != bar_v(t as u32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if baz(t as u32) != baz_v(t as u32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = t;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         t = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
