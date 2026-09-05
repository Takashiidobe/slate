#include <stdio.h>

static int check_target(const char *ptr, const char *end, int *tokPtr) {
  int upper = 0;
  *tokPtr   = 7;
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
  int         tok = 0;
  const char *s   = "xyz";
  int         r   = check_target(s, s + 3, &tok);
  printf("%d %d\n", r, tok);
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut tok: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     tok = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = check_target({{__v[0-9]+}}, {{__v[0-9]+}}, std::ptr::addr_of_mut!(tok));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = tok;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(
// COMMON-LOWERING-NEXT:         0,
// COMMON-LOWERING-NEXT:         0,
// COMMON-LOWERING-NEXT:         std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.4 = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.0 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:             {{__slate_alloca_frame[0-9]+}}.1 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.1;
// COMMON-LOWERING-NEXT:             return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         {
// COMMON-LOWERING-NEXT:             let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-NEXT:                 120 => 0,
// COMMON-LOWERING-NEXT:                 88 => 1,
// COMMON-LOWERING-NEXT:                 _ => 2,
// COMMON-LOWERING-NEXT:             };
// COMMON-LOWERING-NEXT:             '__switch0: loop {
// COMMON-LOWERING-NEXT:                 match __switch_case0 {
// COMMON-LOWERING-NEXT:                     0 => {
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     1 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                         {{__slate_alloca_frame[0-9]+}}.0 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     2 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                         {{__slate_alloca_frame[0-9]+}}.1 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.1;
// COMMON-LOWERING-NEXT:                         return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     _ => {
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 9;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2;
// COMMON-LOWERING-NEXT:             unsafe {
// COMMON-LOWERING-NEXT:                 *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.1;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: struct __SlateAllocaFrame0(i32, i32, *mut i32, *mut i8, *mut i8);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"xyz\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT: fn check_target({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.3;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: struct __SlateAllocaFrame0(i32, i32, *mut i32, *mut u8, *mut u8);
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"xyz\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT: fn check_target({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.4;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut tok: i32 = 0;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             check_target({{__v[0-9]+}}, {{__v[0-9]+}}, unsafe { &mut (*std::ptr::addr_of_mut!(tok)) }),
// COMMON-REWRITES-NEXT:             tok,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(
// COMMON-REWRITES-NEXT:         0,
// COMMON-REWRITES-NEXT:         0,
// COMMON-REWRITES-NEXT:         std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.4 = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = {{arg[0-9]+}} as *mut i32;
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.0 = 0;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__slate_alloca_frame[0-9]+}}.2 = 7;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} != 3 {
// COMMON-REWRITES-NEXT:         {{__slate_alloca_frame[0-9]+}}.1 = 1;
// COMMON-REWRITES-NEXT:         return {{__slate_alloca_frame[0-9]+}}.1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     match (unsafe { *{{__v[0-9]+}} }) as i32 {
// COMMON-REWRITES-NEXT:         120 => {}
// COMMON-REWRITES-NEXT:         88 => {
// COMMON-REWRITES-NEXT:             {{__slate_alloca_frame[0-9]+}}.0 = 1;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         _ => {
// COMMON-REWRITES-NEXT:             {{__slate_alloca_frame[0-9]+}}.1 = 1;
// COMMON-REWRITES-NEXT:             return {{__slate_alloca_frame[0-9]+}}.1;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     if {{__slate_alloca_frame[0-9]+}}.0 != 0 {
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             *{{__slate_alloca_frame[0-9]+}}.2 = 9;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = 0;
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.1
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: struct __SlateAllocaFrame0(i32, i32, *mut i32, *mut i8, *mut i8);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"xyz".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-X86_64-GNU-NEXT: fn check_target({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: &mut i32) -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: struct __SlateAllocaFrame0(i32, i32, *mut i32, *mut u8, *mut u8);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"xyz".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-AARCH64-GNU-NEXT: fn check_target({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: &mut i32) -> i32 {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.4;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.4;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
