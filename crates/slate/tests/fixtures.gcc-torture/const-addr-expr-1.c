#include <stdio.h>
#include <stdlib.h>
extern void abort();

typedef struct foo {
  int   uaattrid;
  char *name;
} FOO;

FOO Upgrade_items[] = {{1, "1"}, {2, "2"}, {0, NULL}};

int *Upgd_minor_ID = (int *)&((Upgrade_items + 1)->uaattrid);

int *Upgd_minor_ID1 = (int *)&((Upgrade_items)->uaattrid);

int main(int argc, char **argv) {
  if (*Upgd_minor_ID != 2)
    abort();

  if (*Upgd_minor_ID1 != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct foo {
// CHECK-NEXT:     uaattrid: i32,
// CHECK-NEXT:     name: *mut i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut Upgrade_items: __SlateAlign16<[foo; 3]> = __SlateAlign16([
// CHECK-NEXT:     foo {
// CHECK-NEXT:         uaattrid: 1 as i32,
// CHECK-NEXT:         name: b"1\0".as_ptr() as *mut i8,
// CHECK-NEXT:     },
// CHECK-NEXT:     foo {
// CHECK-NEXT:         uaattrid: 2 as i32,
// CHECK-NEXT:         name: b"2\0".as_ptr() as *mut i8,
// CHECK-NEXT:     },
// CHECK-NEXT:     foo {
// CHECK-NEXT:         uaattrid: 0 as i32,
// CHECK-NEXT:         name: std::ptr::null_mut::<i8>(),
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut Upgd_minor_ID: *mut i32 = unsafe {
// CHECK-NEXT:     std::ptr::addr_of_mut!(
// CHECK-NEXT:         (* unsafe { unsafe { std::ptr::addr_of_mut!(Upgrade_items.0) as * mut foo }
// CHECK-NEXT:         .offset((1 as i32) as isize) }).uaattrid
// CHECK-NEXT:     )
// CHECK-NEXT: };
// CHECK-NEXT: static mut Upgd_minor_ID1: *mut i32 = unsafe {
// CHECK-NEXT:     std::ptr::addr_of_mut!(
// CHECK-NEXT:         (* unsafe { std::ptr::addr_of_mut!(Upgrade_items.0) as * mut foo }).uaattrid
// CHECK-NEXT:     )
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     if (unsafe { *unsafe { Upgd_minor_ID } }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { Upgd_minor_ID1 } }) != (1 as i32) {
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
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
