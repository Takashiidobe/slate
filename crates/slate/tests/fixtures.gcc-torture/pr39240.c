/* PR target/39240 */

extern void abort(void);

__attribute__((noinline)) static int foo1(int x) { return x; }

__attribute__((noinline)) unsigned int bar1(int x) { return foo1(x + 6); }

volatile unsigned long l1 = (unsigned int)-4;

__attribute__((noinline)) static short int foo2(int x) { return x; }

__attribute__((noinline)) unsigned short int bar2(int x) { return foo2(x + 6); }

volatile unsigned long l2 = (unsigned short int)-4;

__attribute__((noinline)) static signed char foo3(int x) { return x; }

__attribute__((noinline)) unsigned char bar3(int x) { return foo3(x + 6); }

volatile unsigned long l3 = (unsigned char)-4;

__attribute__((noinline)) static unsigned int foo4(int x) { return x; }

__attribute__((noinline)) int bar4(int x) { return foo4(x + 6); }

volatile unsigned long l4 = (int)-4;

__attribute__((noinline)) static unsigned short int foo5(int x) { return x; }

__attribute__((noinline)) short int bar5(int x) { return foo5(x + 6); }

volatile unsigned long l5 = (short int)-4;

__attribute__((noinline)) static unsigned char foo6(int x) { return x; }

__attribute__((noinline)) signed char bar6(int x) { return foo6(x + 6); }

volatile unsigned long l6 = (signed char)-4;

int main(void) {
  if (bar1(-10) != l1)
    abort();
  if (bar2(-10) != l2)
    abort();
  if (bar3(-10) != l3)
    abort();
  if (bar4(-10) != l4)
    abort();
  if (bar5(-10) != l5)
    abort();
  if (bar6(-10) != l6)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/39240
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo1(mut x: i32) -> i32 {
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar1(mut x: i32) -> u32 {
// CHECK-NEXT:     foo1(x + (6 as i32)) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut l1: u64 = (-(4 as i32) as u32) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo2(mut x: i32) -> i16 {
// CHECK-NEXT:     x as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar2(mut x: i32) -> u16 {
// CHECK-NEXT:     foo2(x + (6 as i32)) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut l2: u64 = ((-(4 as i32) as i16) as u16) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo3(mut x: i32) -> i8 {
// CHECK-NEXT:     x as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar3(mut x: i32) -> u8 {
// CHECK-NEXT:     foo3(x + (6 as i32)) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut l3: u64 = ((-(4 as i32) as i8) as u8) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo4(mut x: i32) -> u32 {
// CHECK-NEXT:     x as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar4(mut x: i32) -> i32 {
// CHECK-NEXT:     foo4(x + (6 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut l4: u64 = (-(4 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo5(mut x: i32) -> u16 {
// CHECK-NEXT:     (x as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar5(mut x: i32) -> i16 {
// CHECK-NEXT:     foo5(x + (6 as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut l5: u64 = ((-(4 as i32) as i16) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo6(mut x: i32) -> u8 {
// CHECK-NEXT:     (x as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar6(mut x: i32) -> i8 {
// CHECK-NEXT:     foo6(x + (6 as i32)) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut l6: u64 = ((-(4 as i32) as i8) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (bar1(-(10 as i32)) as u64) != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(l1)) } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((((bar2(-(10 as i32)) as u32) as i32) as i64) as u64)
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(l2)) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((((bar3(-(10 as i32)) as u32) as i32) as i64) as u64)
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(l3)) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((bar4(-(10 as i32)) as i64) as u64)
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(l4)) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((bar5(-(10 as i32)) as i32) as i64) as u64)
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(l5)) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((bar6(-(10 as i32)) as i32) as i64) as u64)
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(l6)) }
// CHECK-NEXT:     {
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
