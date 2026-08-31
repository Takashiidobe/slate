#include <ctype.h>
#include <locale.h>
#include <stdio.h>

int main(void) {
  setlocale(LC_ALL, "");
  int lower = 'q';
  int upper = 'Q';
  printf("%d %d\n", toupper(lower), tolower(upper));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn setlocale(_0: i32, _1: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn toupper(_0: i32) -> i32;
// LOWERING-NEXT:     fn tolower(_0: i32) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { setlocale({{_v[0-9]+}} as i32, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 113;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 81;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { toupper({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { tolower({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: unsafe { toupper(
// REWRITES-DAG: unsafe { tolower(
// REWRITES: {{^}}}
