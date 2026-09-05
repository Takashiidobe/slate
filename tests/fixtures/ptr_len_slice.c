#include <stdio.h>

static int sum(int *items, int len) {
  int total = 0;
  for (int i = 0; i < len; i++) {
    total += items[i];
  }
  return total;
}

static void bump(int *items, int len) {
  for (int i = 0; i < len; i++) {
    items[i] += 1;
  }
}

int main(void) {
  int values[4] = {2, 4, 6, 8};
  printf("%d\n", sum(values, 4));
  bump(values, 4);
  printf("%d %d\n", values[0], values[3]);
  return 0;
}

// REWRITES-NOT: __slate_item
// REWRITES-NOT: items.as_mut_ptr()

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [0; 4];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [2, 4, 6, 8];
// LOWERING-X86_64-GNU-NEXT:     *values = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     values = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     bump({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sum({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut items: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut len: i32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     items = {{arg[0-9]+}};
// LOWERING-NEXT:     len = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = len;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:                 total = {{__v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn bump({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut items: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut len: i32 = 0;
// LOWERING-NEXT:     items = {{arg[0-9]+}};
// LOWERING-NEXT:     len = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = len;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:     *values = [2, 4, 6, 8];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [2, 4, 6, 8];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d\n".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             sum(unsafe { std::slice::from_raw_parts({{__v[0-9]+}} as *const i32, (4 as i32) as usize) }),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     bump(unsafe { std::slice::from_raw_parts_mut({{__v[0-9]+}} as *mut i32, (4 as i32) as usize) });
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), values[0], values[3]) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum({{arg[0-9]+}}: &[i32]) -> i32 {
// REWRITES-NEXT:     let mut items: *mut i32 = {{arg[0-9]+}}.as_ptr() as *mut i32;
// REWRITES-NEXT:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     for i in 0..len {
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:         total += unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     total
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn bump({{arg[0-9]+}}: &mut [i32]) {
// REWRITES-NEXT:     let mut items: *mut i32 = {{arg[0-9]+}}.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-NEXT:     for i in 0..len {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *{{__v[0-9]+}} = (unsafe { *{{__v[0-9]+}} }) + {{__v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
