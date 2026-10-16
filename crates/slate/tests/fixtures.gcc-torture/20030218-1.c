/*  On H8, the predicate general_operand_src(op,mode) used to ignore
    mode when op is a (mem (post_inc ...)).  As a result, the pattern
    for extendhisi2 was recognized as extendqisi2.  */

extern void abort();
extern void exit(int);

short *q;

long foo(short *p) {
  long b = *p;
  q      = p + 1;
  return b;
}

int main() {
  short a = 0xff00;
  if (foo(&a) != (long)(short)0xff00)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //!  On H8, the predicate general_operand_src(op,mode) used to ignore
// CHECK-NEXT: //! mode when op is a (mem (post_inc ...)).  As a result, the pattern
// CHECK-NEXT: //! for extendhisi2 was recognized as extendqisi2.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut q: *mut i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: *mut i16) -> i64 {
// CHECK-NEXT:     let mut b: i64 = (unsafe { *p }) as i64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         q = unsafe { p.offset((1 as i32) as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i16 = (65280 as i32) as i16;
// CHECK-NEXT:     if foo(std::ptr::addr_of_mut!(a)) != (((65280 as i32) as i16) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
