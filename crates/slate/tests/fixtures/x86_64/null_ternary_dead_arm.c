#include <stddef.h>
#include <stdio.h>

typedef struct arena { unsigned long idx; } arena_t;

__attribute__((noinline)) static unsigned long start(void) { return 7; }

#define START_OR_INDEX(req) ((req) == NULL ? start() : ((arena_t *)(req))->idx)

int main(void) {
  arena_t arena = {42};
  void *given = &arena;
  printf("%lu %lu\n", START_OR_INDEX(NULL), START_OR_INDEX(given));
  printf("%lu\n", (NULL != NULL) ? ((arena_t *)NULL)->idx : start() + 1);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! START_OR_INDEX {
// CHECK-NEXT:     () => {
// CHECK-NEXT:         { let mut __v : u64; __v = start(); __v }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! START_OR_INDEX__2 {
// CHECK-NEXT:     ($req:expr) => {
// CHECK-NEXT:         { let __v : u64 = if $req .is_null() { start() } else { unsafe { (* (($req) as *
// CHECK-NEXT:         mut arena)).idx } }; __v }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct arena {
// CHECK-NEXT:     idx: u64,
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn start() -> u64 {
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut arena: arena = arena { idx: 42 };
// CHECK-NEXT:     let given: *mut () = std::ptr::addr_of_mut!(arena) as *mut ();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lu %lu\n".as_ptr(),
// CHECK-NEXT:             START_OR_INDEX!(),
// CHECK-NEXT:             START_OR_INDEX__2!(given),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64;
// CHECK-NEXT:     [[V_0]] = start().wrapping_add(1);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lu\n".as_ptr(), [[V_0]]) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
