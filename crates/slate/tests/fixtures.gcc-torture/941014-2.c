#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned short a;
  unsigned short b;
} foo_t;

void a1(unsigned long offset) {}

volatile foo_t *f() {
  volatile foo_t *foo_p = (volatile foo_t *)malloc(sizeof(foo_t));

  a1((unsigned long)foo_p - 30);
  if (foo_p->a & 0xf000)
    printf("%d\n", foo_p->a);
  foo_p->b = 0x0100;
  a1((unsigned long)foo_p + 2);
  a1((unsigned long)foo_p - 30);
  return foo_p;
}

int main(void) {
  volatile foo_t *foo_p;

  foo_p = f();
  if (foo_p->b != 0x0100)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: u16,
// CHECK-NEXT:     b: u16,
// CHECK-NEXT: }
// CHECK-NEXT: fn a1(mut offset: u64) {}
// CHECK-NEXT: fn f() -> *mut [[SLATERECORD_0]] {
// CHECK-NEXT:     let mut foo_p: *mut [[SLATERECORD_0]] = (unsafe { malloc(4 as u64) })
// CHECK-NEXT:         as *mut [[SLATERECORD_0]];
// CHECK-NEXT:     a1((foo_p as u64).wrapping_sub(((30 as i32) as i64) as u64));
// CHECK-NEXT:     if (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!((* foo_p).a)) }) as u32)
// CHECK-NEXT:         as i32) & (61440 as i32) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d\n".as_ptr() as *const i8,
// CHECK-NEXT:                 ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!((* foo_p).a)) })
// CHECK-NEXT:                     as u32) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!((* foo_p).b),
// CHECK-NEXT:             ((256 as i32) as i16) as u16,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     a1((foo_p as u64).wrapping_add(((2 as i32) as i64) as u64));
// CHECK-NEXT:     a1((foo_p as u64).wrapping_sub(((30 as i32) as i64) as u64));
// CHECK-NEXT:     foo_p
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut foo_p: *mut [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     foo_p = f();
// CHECK-NEXT:     if (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!((* foo_p).b)) }) as u32)
// CHECK-NEXT:         as i32) != (256 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
