#include <stdio.h>

int missing(int a) __attribute__((weak));
void also_missing(void) __attribute__((weak));

int main(void) {
    int result = 7;
    if (missing)
        result = missing(3);
    if (also_missing != 0)
        also_missing();
    printf("%d %d\n", result, missing == 0);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(linkage)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     #[linkage = "extern_weak"]
// CHECK-NEXT:     static missing: Option<unsafe extern "C-unwind" fn(i32) -> i32>;
// CHECK-NEXT:     #[linkage = "extern_weak"]
// CHECK-NEXT:     static also_missing: Option<unsafe extern "C-unwind" fn()>;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut result: i32 = 7;
// CHECK-NEXT:     if (unsafe { missing }) != None {
// CHECK-NEXT:         result = unsafe { missing.unwrap()(3i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { also_missing }) != None {
// CHECK-NEXT:         unsafe { also_missing.unwrap()() };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", result, ((unsafe { missing }) == None) as i32);
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
