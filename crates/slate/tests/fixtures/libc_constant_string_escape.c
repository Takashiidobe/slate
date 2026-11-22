#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void pick(const char **out) { *out = "beta"; }

int main(void) {
  const char *word = "alpha";
  pick(&word);
  printf("%d %d\n", strcmp(word, "alpha") == 0, (int)strlen(word));
  const char *fixed = "alpha";
  const char *alias = fixed;
  printf("%d %d %d\n", strcmp(alias, "alpha") == 0, (int)strlen(alias),
         atoi("  -42x"));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::strcmp;
// CHECK-NEXT: use slate_support::strlen;
// CHECK-EMPTY:
// CHECK-NEXT: fn pick(out: &mut *const i8) {
// CHECK-NEXT:     *out = c"beta".as_ptr();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut word: *const i8 = c"alpha".as_ptr();
// CHECK-NEXT:     pick(&mut word);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         ((unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 word as *const core::ffi::c_char,
// CHECK-NEXT:                 c"alpha".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:             ) as i32
// CHECK-NEXT:         }) == 0) as i32,
// CHECK-NEXT:         unsafe { (strlen(word as *const core::ffi::c_char) as u64) as i32 }
// CHECK-NEXT:     );
// CHECK-NEXT:     let fixed: *const i8 = c"alpha".as_ptr();
// CHECK-NEXT:     let alias: &str = "alpha";
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         (alias == "alpha") as i32,
// CHECK-NEXT:         (alias.len() as u64) as i32,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::__slate_atoi(
// CHECK-NEXT:                 (c"  -42x".as_ptr() as *const core::ffi::c_char) as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
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
