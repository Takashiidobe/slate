#include <stdio.h>

__attribute__((nonnull(1, 3))) static int combine(int *left, int scale,
                                                  int *right, int *optional) {
  return *left + scale * *right + (optional ? *optional : 0);
}

__attribute__((nonnull)) static int difference(int *left, int scale,
                                               int *right) {
  return *left - scale * *right;
}

int main(void) {
  int left  = 11;
  int right = 3;
  printf("%d %d\n", combine(&left, 2, &right, NULL),
         difference(&left, 2, &right));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn combine({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut optional: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     optional = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = optional;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = optional;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn difference({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut left: i32 = 0;
// LOWERING-NEXT:     let mut right: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 11;
// LOWERING-NEXT:     left = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     right = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = combine(std::ptr::addr_of_mut!(left), {{_v[0-9]+}}, std::ptr::addr_of_mut!(right), {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = difference(std::ptr::addr_of_mut!(left), {{_v[0-9]+}}, std::ptr::addr_of_mut!(right));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn combine({{arg[0-9]+}}: &i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: &i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-NEXT: let mut optional: *mut i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = (unsafe { *({{arg[0-9]+}} as *const i32) }) + {{arg[0-9]+}} * unsafe { *({{arg[0-9]+}} as *const i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = optional != std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { *optional };
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: return {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn difference({{arg[0-9]+}}: &i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: &i32) -> i32 {
// REWRITES-NEXT: return (unsafe { *({{arg[0-9]+}} as *const i32) }) - {{arg[0-9]+}} * unsafe { *({{arg[0-9]+}} as *const i32) };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut left: i32 = 0;
// REWRITES-NEXT: let mut right: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: left = 11;
// REWRITES-NEXT: right = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = combine(unsafe { &(*std::ptr::addr_of_mut!(left)) }, {{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(right)) }, {{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = difference(unsafe { &(*std::ptr::addr_of_mut!(left)) }, {{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(right)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
