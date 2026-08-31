#include <stdio.h>

int main(void) {
  char buf[64];
  sprintf(buf, "%d-%d", 3, 4);
  buf[0] = 'X';
  puts(buf);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn sprintf(_0: *mut i8, _1: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn puts(_0: *const i8) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d-%d\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { sprintf({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 88;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     buf[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { puts({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn sprintf(_0: *mut i8, _1: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn puts(_0: *const i8) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut buf: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d-%d\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { sprintf({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: buf[({{_v[0-9]+}} as usize)] = 88;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { puts({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
