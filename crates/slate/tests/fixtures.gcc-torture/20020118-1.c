/* This tests an insn length of sign extension on h8300 port.  */

extern void exit(int);

volatile signed char *q;
volatile signed int   n;

void foo(void) {
  signed char *p;

  for (;;) {
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
  }
}

int main() { exit(0); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This tests an insn length of sign extension on h8300 port.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut q: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut n: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     let mut p: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         p = (unsafe { q }) as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n),
// CHECK-NEXT:                 (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         p = (unsafe { q }) as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n),
// CHECK-NEXT:                 (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         p = (unsafe { q }) as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n),
// CHECK-NEXT:                 (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         p = (unsafe { q }) as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n),
// CHECK-NEXT:                 (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         p = (unsafe { q }) as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n),
// CHECK-NEXT:                 (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         p = (unsafe { q }) as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n),
// CHECK-NEXT:                 (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         p = (unsafe { q }) as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n),
// CHECK-NEXT:                 (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         p = (unsafe { q }) as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n),
// CHECK-NEXT:                 (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         p = (unsafe { q }) as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n),
// CHECK-NEXT:                 (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         p = (unsafe { q }) as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n),
// CHECK-NEXT:                 (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         p = (unsafe { q }) as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n),
// CHECK-NEXT:                 (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
