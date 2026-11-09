

#include <stdio.h>

int pragma_weak_target(int value) { return value + 7; }

#pragma weak pragma_weak_alias = pragma_weak_target
extern int   pragma_weak_alias(int);

#pragma redefine_extname pragma_renamed pragma_actual
int pragma_renamed(int value) { return value * 3; }

int main(void) {
  printf("%d %d %d\n", pragma_weak_alias(29),
         pragma_weak_alias == pragma_weak_target, pragma_renamed(13));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.pragma_weak_target")]
// CHECK-NEXT: extern "C-unwind" fn pragma_weak_target(value: i32) -> i32 {
// CHECK-NEXT:     value + 7
// CHECK-NEXT: }
// CHECK-NEXT: fn pragma_renamed(value: i32) -> i32 {
// CHECK-NEXT:     value * 3
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", pragma_weak_target(29i32), (({ let __slate_function : Option < unsafe
// CHECK-NEXT:         extern "C-unwind" fn (i32) -> i32 > = Some(pragma_weak_target); __slate_function
// CHECK-NEXT:         }) == Some(pragma_weak_target)) as i32, pragma_renamed(13i32)
// CHECK-NEXT:     );
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
