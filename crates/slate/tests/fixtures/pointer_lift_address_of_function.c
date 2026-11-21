#include <stdio.h>

static int first(const char *line, int show) { return show + (line[0] == 'x'); }

static int second(const char *line, int show) { return show * (line[1] == 'y'); }

int main(void) {
  int (*pick)(const char *, int) = &first;
  int direct                     = second("xy", 5);
  printf("%d %d\n", pick("x", 4), direct);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.first")]
// CHECK-NEXT: extern "C-unwind" fn first(line: *const i8, show: i32) -> i32 {
// CHECK-NEXT:     show + ((((unsafe { *line.add(0) }) as i32) == 120) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn second(line: *const i8, show: i32) -> i32 {
// CHECK-NEXT:     show * ((((unsafe { *line.add(1) }) as i32) == 121) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let pick: Option<unsafe extern "C-unwind" fn(*const i8, i32) -> i32> = Some(first);
// CHECK-NEXT:     let direct: i32 = second(c"xy".as_ptr(), 5i32);
// CHECK-NEXT:     println!("{} {}", unsafe { pick.unwrap()(c"x".as_ptr(), 4i32) }, direct);
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
