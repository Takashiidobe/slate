#include <stdio.h>

__attribute__((destructor)) static void cleanup(void) {
  printf("destructor ran\n");
}

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("early exit\n");
    return 7;
  }
  printf("main ran\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn cleanup() {
// CHECK-NEXT:     println!("destructor ran");
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn __slate_fini_cleanup() {
// CHECK-NEXT:     cleanup();
// CHECK-NEXT: }
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used]
// CHECK-NEXT: #[unsafe(link_section = ".fini_array")]
// CHECK-NEXT: static __SLATE_FINI_cleanup: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_fini_cleanup,
// CHECK-NEXT: );
// CHECK-NEXT: fn __slate_main(argc: i32, argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     if argc == 1 {
// CHECK-NEXT:         println!("early exit");
// CHECK-NEXT:         return 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("main ran");
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
