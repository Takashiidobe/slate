#include <stddef.h>
#include <stdio.h>

static int target(void) { return 42; }

int main(void) {
  int       (*base)(void) = target;
  ptrdiff_t forward       = (base + 3) - base;
  ptrdiff_t backward      = (base - 2) - base;
  ptrdiff_t difference    = base - (base + 3);
  int       unchanged     = base + 0 == base;
  printf("%td %td %td %d\n", forward, backward, difference, unchanged);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: struct __SlateAllocaFrame0(i32, i64, i64, i64, Option<unsafe extern "C" fn() -> i32>, i32);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn target() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(0, 0, 0, 0, None, 0);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.5 = {{_v[0-9]+}};
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.4 = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn() -> i32>>(target as *const ()) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = unsafe { std::mem::transmute::<*const u8, Option<unsafe extern "C" fn() -> i32>>(unsafe { ({{_v[0-9]+}}.unwrap() as *const u8).add(3) }) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { ({{_v[0-9]+}}.unwrap() as *const u8).offset_from({{_v[0-9]+}}.unwrap() as *const u8) as i64 };
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = unsafe { std::mem::transmute::<*const u8, Option<unsafe extern "C" fn() -> i32>>(unsafe { ({{_v[0-9]+}}.unwrap() as *const u8).offset({{_v[0-9]+}} as isize) }) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { ({{_v[0-9]+}}.unwrap() as *const u8).offset_from({{_v[0-9]+}}.unwrap() as *const u8) as i64 };
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = unsafe { std::mem::transmute::<*const u8, Option<unsafe extern "C" fn() -> i32>>(unsafe { ({{_v[0-9]+}}.unwrap() as *const u8).add(3) }) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { ({{_v[0-9]+}}.unwrap() as *const u8).offset_from({{_v[0-9]+}}.unwrap() as *const u8) as i64 };
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = unsafe { std::mem::transmute::<*const u8, Option<unsafe extern "C" fn() -> i32>>(unsafe { ({{_v[0-9]+}}.unwrap() as *const u8).add(0) }) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.0 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%td %td %td %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{__slate_alloca_frame[0-9]+}}.3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{__slate_alloca_frame[0-9]+}}.2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.5 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.5;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: struct __SlateAllocaFrame0(i32, i64, i64, i64, Option<unsafe extern "C" fn() -> i32>, i32);
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn target() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 42;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(0, 0, 0, 0, None, 0);
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.5 = 0;
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.4 = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn() -> i32>>(target as *const ()) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = unsafe { std::mem::transmute::<*const u8, Option<unsafe extern "C" fn() -> i32>>(unsafe { ({{__slate_alloca_frame[0-9]+}}.4.unwrap() as *const u8).add(3) }) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { ({{_v[0-9]+}}.unwrap() as *const u8).offset_from({{__slate_alloca_frame[0-9]+}}.4.unwrap() as *const u8) as i64 };
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.3 = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = -2;
// REWRITES-NEXT: let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = unsafe { std::mem::transmute::<*const u8, Option<unsafe extern "C" fn() -> i32>>(unsafe { ({{__slate_alloca_frame[0-9]+}}.4.unwrap() as *const u8).offset({{_v[0-9]+}} as isize) }) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { ({{_v[0-9]+}}.unwrap() as *const u8).offset_from({{__slate_alloca_frame[0-9]+}}.4.unwrap() as *const u8) as i64 };
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.2 = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = {{__slate_alloca_frame[0-9]+}}.4;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = unsafe { std::mem::transmute::<*const u8, Option<unsafe extern "C" fn() -> i32>>(unsafe { ({{__slate_alloca_frame[0-9]+}}.4.unwrap() as *const u8).add(3) }) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { ({{_v[0-9]+}}.unwrap() as *const u8).offset_from({{_v[0-9]+}}.unwrap() as *const u8) as i64 };
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: Option<unsafe extern "C" fn() -> i32> = unsafe { std::mem::transmute::<*const u8, Option<unsafe extern "C" fn() -> i32>>(unsafe { ({{__slate_alloca_frame[0-9]+}}.4.unwrap() as *const u8).add(0) }) };
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.0 = ({{_v[0-9]+}} == {{__slate_alloca_frame[0-9]+}}.4) as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%td %td %td %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = {{__slate_alloca_frame[0-9]+}}.3;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = {{__slate_alloca_frame[0-9]+}}.2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.5 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.5;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
