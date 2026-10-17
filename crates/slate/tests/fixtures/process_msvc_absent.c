#include <stdlib.h>
#include <wchar.h>

extern int _beginthread;
extern int _cwait;
extern int _execv;
extern int _get_initial_narrow_environment;
extern int _getpid;
extern int _spawnv;
extern int _wexecv;
extern int _wspawnv;

int main(void) { return 0; }

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut _beginthread: i32;
// CHECK-NEXT:     static mut _cwait: i32;
// CHECK-NEXT:     static mut _execv: i32;
// CHECK-NEXT:     static mut _get_initial_narrow_environment: i32;
// CHECK-NEXT:     static mut _getpid: i32;
// CHECK-NEXT:     static mut _spawnv: i32;
// CHECK-NEXT:     static mut _wexecv: i32;
// CHECK-NEXT:     static mut _wspawnv: i32;
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
