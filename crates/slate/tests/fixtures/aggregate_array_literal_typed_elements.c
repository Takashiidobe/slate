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
// LOWERING-NEXT:     name: *mut i8,
// LOWERING-NEXT:     value: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn count_null_pairs() -> i32 {
// LOWERING-NEXT:     let mut pairs: aligned::Aligned<aligned::A16, [pair; 1]> = aligned::Aligned(
// LOWERING-NEXT:         [pair {
// LOWERING-NEXT:             name: std::ptr::null_mut(),
// LOWERING-NEXT:             value: std::ptr::null_mut(),
// LOWERING-NEXT:         }; 1],
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     *pairs = [pair {
// LOWERING-NEXT:         name: std::ptr::null_mut(),
// LOWERING-NEXT:         value: std::ptr::null_mut(),
// LOWERING-NEXT:     }; 1];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = pairs[({{_v[0-9]+}} as usize)].name;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut i8 = pairs[({{_v[0-9]+}} as usize)].value;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                         {{_v[0-9]+}}
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:                         {{_v[0-9]+}}
// LOWERING-NEXT:                     };
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                         total = {{_v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn count_true_flags() -> i32 {
// LOWERING-NEXT:     let mut values: [bool; 2] = [false; 2];
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     values = [true, false];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: u64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = 2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = i;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                         total = {{_v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = count_null_pairs();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = count_true_flags();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT:     name: *mut i8,
// REWRITES-NEXT:     value: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn count_null_pairs() -> i32 {
// REWRITES-NEXT:     let mut pairs: aligned::Aligned<aligned::A16, [pair; 1]> = aligned::Aligned(
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
// REWRITES-NEXT:     total = 0;
// REWRITES-NEXT:     let mut i: i32 = 0;
// REWRITES-NEXT:     i = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:         if i >= {{_v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = if pairs[((i as i64) as usize)].name == {{_v[0-9]+}} {
// REWRITES-NEXT:             let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:             let {{_v[0-9]+}}: bool = pairs[((i as i64) as usize)].value == {{_v[0-9]+}};
// REWRITES-NEXT:             {{_v[0-9]+}}
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:             {{_v[0-9]+}}
// REWRITES-NEXT:         };
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:             total = total + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         i = i + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn count_true_flags() -> i32 {
// REWRITES-NEXT:     let mut values: [bool; 2] = [false; 2];
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     values = [true, false];
// REWRITES-NEXT:     total = 0;
// REWRITES-NEXT:     let mut i: u64 = 0;
// REWRITES-NEXT:     i = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 2;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT:         if i >= {{_v[0-9]+}} / {{_v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         if values[(i as usize)] {
// REWRITES-NEXT:             total = total + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         i = i + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), count_null_pairs(), count_true_flags()) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
