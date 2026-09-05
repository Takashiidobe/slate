// { dg-additional-options "-O1" }
#include <stdio.h>

static int side_effect_calls = 0;

static int bump(int v) {
  side_effect_calls++;
  return v;
}

static int use_expect(int x) {
  if (__builtin_expect(bump(x), 1)) {
    return 1;
  }
  return 0;
}

static int use_expect_with_probability(int x) {
  if (__builtin_expect_with_probability(bump(x), 1, 0.9)) {
    return 1;
  }
  return 0;
}

static int use_unpredictable(int x) {
  if (__builtin_unpredictable(bump(x) > 0)) {
    return 1;
  }
  return 0;
}

int main(void) {
  int a = use_expect(1);
  int b = use_expect_with_probability(1);
  int c = use_unpredictable(1);
  printf("%d %d %d %d\n", a, b, c, side_effect_calls);
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
// COMMON-LOWERING-NEXT: static mut side_effect_calls: i32 = 0;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut a: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut b: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut c: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = use_expect({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = use_expect_with_probability({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:             b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = use_unpredictable({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                 c = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = a;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = b;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = c;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { side_effect_calls };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                 std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn use_expect({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut x: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     x = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = bump({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn use_expect_with_probability({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut x: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     x = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = bump({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn use_unpredictable({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut x: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     x = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = bump({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn bump({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { side_effect_calls };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         side_effect_calls = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return {{arg[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: static mut side_effect_calls: i32 = 0;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut __retval: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut a: i32 = use_expect(1);
// COMMON-REWRITES-NEXT:     let mut b: i32 = use_expect_with_probability(1);
// COMMON-REWRITES-NEXT:     let mut c: i32 = use_unpredictable(1);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(c"%d %d %d %d\n".as_ptr(), a, b, c, unsafe {
// COMMON-REWRITES-NEXT:             side_effect_calls
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     __retval = 0;
// COMMON-REWRITES-NEXT:     std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:     std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn use_expect(mut {{__v[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = bump({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:     if ({{__v[0-9]+}} as i64) != 0 {
// COMMON-REWRITES-NEXT:         return 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     0
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn use_expect_with_probability(mut {{__v[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = bump({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:     if ({{__v[0-9]+}} as i64) != 0 {
// COMMON-REWRITES-NEXT:         return 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     0
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn use_unpredictable(mut {{__v[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = bump({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} > 0 {
// COMMON-REWRITES-NEXT:         return 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     0
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn bump({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         side_effect_calls = (unsafe { side_effect_calls }) + 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
