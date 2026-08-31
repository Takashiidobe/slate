#include <stdio.h>

static int check_target(const char *ptr, const char *end, int *tokPtr) {
  int upper = 0;
  *tokPtr = 7;
  if (end - ptr != 3)
    return 1;
  switch (ptr[0]) {
  case 'x':
    break;
  case 'X':
    upper = 1;
    break;
  default:
    return 1;
  }
  if (upper)
    *tokPtr = 9;
  return 0;
}

int main(void) {
  int tok = 0;
  const char *s = "xyz";
  int r = check_target(s, s + 3, &tok);
  printf("%d %d\n", r, tok);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: struct __SlateAllocaFrame0(i32, i32, *mut i32, *mut i8, *mut i8);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_target({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(0, 0, std::ptr::null_mut(), std::ptr::null_mut(), std::ptr::null_mut());
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.4 = {{arg[0-9]+}};
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{arg[0-9]+}};
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.0 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.3;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-NEXT:             return {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 { 120 => 0, 88 => 1, _ => 2 };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                         {{__slate_alloca_frame[0-9]+}}.0 = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                         {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-NEXT:                         return {{_v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 9;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut tok: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     tok = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"xyz\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(3) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = check_target({{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(tok));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = tok;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: struct __SlateAllocaFrame0(i32, i32, *mut i32, *mut i8, *mut i8);
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_target({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: &mut i32) -> i32 {
// REWRITES-NEXT: let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(0, 0, std::ptr::null_mut(), std::ptr::null_mut(), std::ptr::null_mut());
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.4 = {{arg[0-9]+}};
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.3 = {{arg[0-9]+}};
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.2 = {{arg[0-9]+}} as *mut i32;
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.0 = 0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *{{__slate_alloca_frame[0-9]+}}.2 = 7;
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.3;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = 3;
// REWRITES-NEXT:         if {{_v[0-9]+}} != {{_v[0-9]+}} {
// REWRITES-NEXT:                     {{__slate_alloca_frame[0-9]+}}.1 = 1;
// REWRITES-NEXT:                     return {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let __switch_value0 = (unsafe { *{{_v[0-9]+}} }) as i32;
// REWRITES-NEXT:                     let mut __switch_case0: i32 = match __switch_value0 { 120 => 0, 88 => 1, _ => 2 };
// REWRITES-NEXT:                     '__switch0: loop {
// REWRITES-NEXT:                                     match __switch_case0 {
// REWRITES-NEXT:                                         0 => {
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         1 => {
// REWRITES-NEXT:                                                             {{__slate_alloca_frame[0-9]+}}.0 = 1;
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         2 => {
// REWRITES-NEXT:                                                             {{__slate_alloca_frame[0-9]+}}.1 = 1;
// REWRITES-NEXT:                                                             return {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         _ => {
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{__slate_alloca_frame[0-9]+}}.0 != 0;
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     *{{__slate_alloca_frame[0-9]+}}.2 = 9;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.1 = 0;
// REWRITES-NEXT: return {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut tok: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: tok = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"xyz\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(3) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = check_target({{_v[0-9]+}}, {{_v[0-9]+}}, unsafe { &mut (*std::ptr::addr_of_mut!(tok)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, tok) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
