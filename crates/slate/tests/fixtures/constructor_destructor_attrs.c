#include <stdio.h>

__attribute__((constructor)) static void register_default(void) {
  printf("ctor: default\n");
}

__attribute__((destructor)) static void cleanup_default(void) {
  printf("dtor: default\n");
}

int main(void) {
  printf("main\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn register_default() {
// CHECK-NEXT:     println!("ctor: default");
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn __slate_init_register_default() {
// CHECK-NEXT:     register_default();
// CHECK-NEXT: }
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used]
// CHECK-NEXT: #[unsafe(link_section = ".init_array")]
// CHECK-NEXT: static __SLATE_INIT_register_default: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_init_register_default,
// CHECK-NEXT: );
// CHECK-NEXT: fn cleanup_default() {
// CHECK-NEXT:     println!("dtor: default");
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn __slate_fini_cleanup_default() {
// CHECK-NEXT:     cleanup_default();
// CHECK-NEXT: }
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used]
// CHECK-NEXT: #[unsafe(link_section = ".fini_array")]
// CHECK-NEXT: static __SLATE_FINI_cleanup_default: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_fini_cleanup_default,
// CHECK-NEXT: );
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("main");
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
