#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

extern int _ctime64_s;
extern int _dupenv_s;
extern int _get_errno;
extern int _itoa_s;
extern int _set_invalid_parameter_handler;
extern int fopen_s;
extern int strcpy_s;
extern int wcscpy_s;

int main(void) { return 0; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut _ctime64_s: i32;
// CHECK-NEXT:     static mut _dupenv_s: i32;
// CHECK-NEXT:     static mut _get_errno: i32;
// CHECK-NEXT:     static mut _itoa_s: i32;
// CHECK-NEXT:     static mut _set_invalid_parameter_handler: i32;
// CHECK-NEXT:     static mut fopen_s: i32;
// CHECK-NEXT:     static mut strcpy_s: i32;
// CHECK-NEXT:     static mut wcscpy_s: i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
