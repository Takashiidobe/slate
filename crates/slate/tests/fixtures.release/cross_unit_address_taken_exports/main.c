#include <stdio.h>

#include "api.h"

static int apply(int (*op)(int, int), int a, int b) { return op(a, b); }

int main(void) {
    struct counter c = {1};
    int (*local_add)(int, int) = add;
    bump(&c, add(2, 3));
    api.bump(&c, api.sub(10, 4));
    printf("%d %d %d\n", c.value, apply(sub, 9, 2), apply(api.add, 4, 5));
    printf("%s %s\n", name(), api.name());
    printf("%d %d\n", local_add == api.add, (int (*)(int, int))sub == api.add);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_types;
// CHECK-NEXT: #[path = "routines.rs"]
// CHECK-NEXT: mod __slate_unit_routines;
// CHECK-EMPTY:
// CHECK-NEXT: use crate::__slate_types::api_routines;
// CHECK-NEXT: use crate::__slate_types::counter;
// CHECK-NEXT: use crate::__slate_unit_routines::add;
// CHECK-NEXT: use crate::__slate_unit_routines::bump;
// CHECK-NEXT: use crate::__slate_unit_routines::name;
// CHECK-NEXT: use crate::__slate_unit_routines::sub;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut api: api_routines;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn apply(op: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>, a: i32, b: i32) -> i32 {
// CHECK-NEXT:     unsafe { op.unwrap()(a, b) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c: counter = counter { value: 1 };
// CHECK-NEXT:     let local_add: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>>(
// CHECK-NEXT:             add as *const (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { bump(std::ptr::addr_of_mut!(c), add(2i32, 3i32)) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe { api.bump }.unwrap()(
// CHECK-NEXT:             std::ptr::addr_of_mut!(c),
// CHECK-NEXT:             unsafe { api.sub }.unwrap()(10i32, 4i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         c.value,
// CHECK-NEXT:         apply(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>>(
// CHECK-NEXT:                     sub as *const (),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:             9i32,
// CHECK-NEXT:             2i32
// CHECK-NEXT:         ),
// CHECK-NEXT:         apply(unsafe { api.add }, 4i32, 5i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { printf(c"%s %s\n".as_ptr(), name(), unsafe { api.name }.unwrap()()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         (local_add == unsafe { api.add }) as i32,
// CHECK-NEXT:         ((unsafe {
// CHECK-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>>(
// CHECK-NEXT:                 sub as *const (),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == unsafe { api.add }) as i32
// CHECK-NEXT:     );
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
