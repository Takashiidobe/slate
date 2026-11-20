#include <stdio.h>

__attribute__((constructor(200))) static void init_late(void) {
  printf("ctor: late (200)\n");
}

__attribute__((constructor(101))) static void init_early(void) {
  printf("ctor: early (101)\n");
}

__attribute__((constructor)) static void init_default(void) {
  printf("ctor: default\n");
}

__attribute__((destructor(200))) static void fini_late(void) {
  printf("dtor: late (200)\n");
}

__attribute__((destructor(101))) static void fini_early(void) {
  printf("dtor: early (101)\n");
}

__attribute__((destructor)) static void fini_default(void) {
  printf("dtor: default\n");
}

int main(void) {
  printf("main\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(used_with_arg)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn init_late() {
// CHECK-NEXT:     println!("ctor: late (200)");
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_init_init_late() {
// CHECK-NEXT:     init_late();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used(compiler)]
// CHECK-NEXT: #[unsafe(link_section = ".init_array.00200")]
// CHECK-NEXT: static __SLATE_INIT_init_late: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_init_init_late,
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: fn init_early() {
// CHECK-NEXT:     println!("ctor: early (101)");
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_init_init_early() {
// CHECK-NEXT:     init_early();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used(compiler)]
// CHECK-NEXT: #[unsafe(link_section = ".init_array.00101")]
// CHECK-NEXT: static __SLATE_INIT_init_early: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_init_init_early,
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: fn init_default() {
// CHECK-NEXT:     println!("ctor: default");
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_init_init_default() {
// CHECK-NEXT:     init_default();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used(compiler)]
// CHECK-NEXT: #[unsafe(link_section = ".init_array")]
// CHECK-NEXT: static __SLATE_INIT_init_default: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_init_init_default,
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: fn fini_late() {
// CHECK-NEXT:     println!("dtor: late (200)");
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_fini_fini_late() {
// CHECK-NEXT:     fini_late();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used(compiler)]
// CHECK-NEXT: #[unsafe(link_section = ".fini_array.00200")]
// CHECK-NEXT: static __SLATE_FINI_fini_late: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_fini_fini_late,
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: fn fini_early() {
// CHECK-NEXT:     println!("dtor: early (101)");
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_fini_fini_early() {
// CHECK-NEXT:     fini_early();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used(compiler)]
// CHECK-NEXT: #[unsafe(link_section = ".fini_array.00101")]
// CHECK-NEXT: static __SLATE_FINI_fini_early: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_fini_fini_early,
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: fn fini_default() {
// CHECK-NEXT:     println!("dtor: default");
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_fini_fini_default() {
// CHECK-NEXT:     fini_default();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used(compiler)]
// CHECK-NEXT: #[unsafe(link_section = ".fini_array")]
// CHECK-NEXT: static __SLATE_FINI_fini_default: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_fini_fini_default,
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("main");
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
