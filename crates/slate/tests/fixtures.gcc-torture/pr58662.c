extern void abort(void);

int          a, c, d;
volatile int b;

static int foo(int p1, short p2) { return p1 / p2; }

int main() {
  char e;
  d = foo(a == 0, (0, 35536));
  e = d % 14;
  b = e && c;
  if (b != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(mut p1: i32, mut p2: i16) -> i32 {
// CHECK-NEXT:     p1 / (p2 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut e: i8 = 0 as i8;
// CHECK-NEXT:     0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         d = foo(((unsafe { a }) == (0 as i32)) as i32, (35536 as i32) as i16);
// CHECK-NEXT:     }
// CHECK-NEXT:     e = ((unsafe { d }) % (14 as i32)) as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(b),
// CHECK-NEXT:             (e != (0 as i8) && (unsafe { c }) != (0 as i32)) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
