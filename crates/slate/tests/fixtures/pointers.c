#include <stdio.h>

static int bump_through_pointer(int value) {
  int  local = value;
  int *ptr   = &local;
  *ptr       = *ptr + 3;
  return local;
}

static int add_into_pointer(int *slot, int amount) {
  *slot = *slot + amount;
  return *slot;
}

static int pick_with_pointer_arithmetic(int index) {
  int  values[4];
  int *ptr  = values;
  values[0] = 4;
  values[1] = 8;
  values[2] = 12;
  values[3] = 16;
  return *(ptr + index);
}

int main(void) {
  int total = 10;
  printf("%d\n", bump_through_pointer(5));
  printf("%d\n", add_into_pointer(&total, 7));
  printf("%d\n", total);
  printf("%d\n", pick_with_pointer_arithmetic(2));
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
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = bump_through_pointer({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = add_into_pointer(std::ptr::addr_of_mut!(total), {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = pick_with_pointer_arithmetic({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn bump_through_pointer({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn add_into_pointer({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn pick_with_pointer_arithmetic({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 12;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{arg[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [0; 4];
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
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut total: i32 = 10;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), bump_through_pointer(5)) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             add_into_pointer(unsafe { &mut (*std::ptr::addr_of_mut!(total)) }, 7),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), total) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), pick_with_pointer_arithmetic(2)) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn bump_through_pointer({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + 3
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn add_into_pointer({{arg[0-9]+}}: &mut i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *({{arg[0-9]+}} as *mut i32) = (unsafe { *({{arg[0-9]+}} as *mut i32) }) + {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { *({{arg[0-9]+}} as *mut i32) }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn pick_with_pointer_arithmetic({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 4;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 12;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 16;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{arg[0-9]+}} as isize) };
// COMMON-REWRITES-NEXT:     unsafe { *{{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [0; 4];
// SLATE-FILECHECK-END rewrites-aarch64-gnu
