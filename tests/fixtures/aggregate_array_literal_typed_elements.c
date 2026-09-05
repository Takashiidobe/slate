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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct pair {
// LOWERING-X86_64-GNU-NEXT:     name: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     value: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     name: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     value: *mut u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = count_null_pairs();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = count_true_flags();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn count_null_pairs() -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut pairs: aligned::Aligned<aligned::A16, [pair; 1]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:     let mut pairs: aligned::Aligned<aligned::A8, [pair; 1]> = aligned::Aligned(
// LOWERING-NEXT:         [pair {
// LOWERING-NEXT:             name: std::ptr::null_mut(),
// LOWERING-NEXT:             value: std::ptr::null_mut(),
// LOWERING-NEXT:         }; 1],
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [pair; 1] = [pair {
// LOWERING-NEXT:         name: std::ptr::null_mut(),
// LOWERING-NEXT:         value: std::ptr::null_mut(),
// LOWERING-NEXT:     }; 1];
// LOWERING-NEXT:     *pairs = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = pairs[({{__v[0-9]+}} as usize)].name;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = pairs[({{__v[0-9]+}} as usize)].name;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-X86_64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut i8 = pairs[({{__v[0-9]+}} as usize)].value;
// LOWERING-X86_64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut u8 = pairs[({{__v[0-9]+}} as usize)].value;
// LOWERING-AARCH64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:                         {{__v[0-9]+}}
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-NEXT:                         {{__v[0-9]+}}
// LOWERING-NEXT:                     };
// LOWERING-NEXT:                     if {{__v[0-9]+}} {
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:                         total = {{__v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
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
// LOWERING-NEXT: fn count_true_flags() -> i32 {
// LOWERING-NEXT:     let mut values: [bool; 2] = [false; 2];
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [bool; 2] = [true, false];
// LOWERING-NEXT:     values = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: u64 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         i = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = 2;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = i;
// LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:                     if {{__v[0-9]+}} {
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:                         total = {{__v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{__v[0-9]+}};
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct pair {
// REWRITES-X86_64-GNU-NEXT:     name: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     value: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     name: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     value: *mut u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), count_null_pairs(), count_true_flags()) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn count_null_pairs() -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let mut pairs: aligned::Aligned<aligned::A16, [pair; 1]> = aligned::Aligned(
// REWRITES-AARCH64-GNU-NEXT:     let mut pairs: aligned::Aligned<aligned::A8, [pair; 1]> = aligned::Aligned(
// REWRITES-NEXT:         [pair {
// REWRITES-NEXT:             name: std::ptr::null_mut(),
// REWRITES-NEXT:             value: std::ptr::null_mut(),
// REWRITES-NEXT:         }; 1],
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     *pairs = [pair {
// REWRITES-NEXT:         name: std::ptr::null_mut(),
// REWRITES-NEXT:         value: std::ptr::null_mut(),
// REWRITES-NEXT:     }; 1];
// REWRITES-NEXT:     for i in 0..1 {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = if pairs[((i as i64) as usize)].name == std::ptr::null_mut() {
// REWRITES-NEXT:             let {{__v[0-9]+}}: bool = pairs[((i as i64) as usize)].value == std::ptr::null_mut();
// REWRITES-NEXT:             {{__v[0-9]+}}
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             let {{__v[0-9]+}}: bool = false;
// REWRITES-NEXT:             {{__v[0-9]+}}
// REWRITES-NEXT:         };
// REWRITES-NEXT:         if {{__v[0-9]+}} {
// REWRITES-NEXT:             total += 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     total
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn count_true_flags() -> i32 {
// REWRITES-NEXT:     let mut values: [bool; 2] = [false; 2];
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     values = [true, false];
// REWRITES-NEXT:     for i in 0..(2 / 1) {
// REWRITES-NEXT:         if values[(i as usize)] {
// REWRITES-NEXT:             total += 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     total
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
