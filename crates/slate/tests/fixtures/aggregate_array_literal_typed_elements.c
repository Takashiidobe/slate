#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

struct pair {
  const char *name;
  const char *value;
};

static int count_null_pairs(void) {
  struct pair pairs[] = {{NULL, NULL}};
  int         total   = 0;
  for (int i = 0; i < 1; i++) {
    if (pairs[i].name == NULL && pairs[i].value == NULL) {
      total++;
    }
  }
  return total;
}

static int count_true_flags(void) {
  bool values[] = {true, false};
  int  total    = 0;
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
    if (values[i]) {
      total++;
    }
  }
  return total;
}

int main(void) {
  printf("%d %d\n", count_null_pairs(), count_true_flags());
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
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct pair {
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = count_null_pairs();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = count_true_flags();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn count_null_pairs() -> i32 {
// COMMON-LOWERING-NEXT:         [pair {
// COMMON-LOWERING-NEXT:             name: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             value: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         }; 1],
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [pair; 1] = [pair {
// COMMON-LOWERING-NEXT:         name: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         value: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     }; 1];
// COMMON-LOWERING-NEXT:     *pairs = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:                     } else {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:                     };
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                         total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn count_true_flags() -> i32 {
// COMMON-LOWERING-NEXT:     let mut values: [bool; 2] = [false; 2];
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [bool; 2] = [true, false];
// COMMON-LOWERING-NEXT:     values = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: u64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = 2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} / {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = i;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                         total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     name: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     value: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut pairs: aligned::Aligned<aligned::A16, [pair; 1]> = aligned::Aligned(
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = pairs[({{__v[0-9]+}} as usize)].name;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut i8 = pairs[({{__v[0-9]+}} as usize)].value;
// LOWERING-X86_64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     name: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     value: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut pairs: aligned::Aligned<aligned::A8, [pair; 1]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = pairs[({{__v[0-9]+}} as usize)].name;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut u8 = pairs[({{__v[0-9]+}} as usize)].value;
// LOWERING-AARCH64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
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
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct pair {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), count_null_pairs(), count_true_flags()) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn count_null_pairs() -> i32 {
// COMMON-REWRITES-NEXT:         [pair {
// COMMON-REWRITES-NEXT:             name: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             value: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         }; 1],
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     *pairs = [pair {
// COMMON-REWRITES-NEXT:         name: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         value: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     }; 1];
// COMMON-REWRITES-NEXT:     for i in 0..1 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = if pairs[((i as i64) as usize)].name == std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: bool = pairs[((i as i64) as usize)].value == std::ptr::null_mut();
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             total += 1;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn count_true_flags() -> i32 {
// COMMON-REWRITES-NEXT:     let mut values: [bool; 2] = [false; 2];
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     values = [true, false];
// COMMON-REWRITES-NEXT:     for i in 0..(2 / 1) {
// COMMON-REWRITES-NEXT:         if values[(i as usize)] {
// COMMON-REWRITES-NEXT:             total += 1;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     name: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     value: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     let mut pairs: aligned::Aligned<aligned::A16, [pair; 1]> = aligned::Aligned(
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     name: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     value: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     let mut pairs: aligned::Aligned<aligned::A8, [pair; 1]> = aligned::Aligned(
// SLATE-FILECHECK-END rewrites-aarch64-gnu
