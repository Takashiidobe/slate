#include <stdio.h>

static void pairwise_swap(int *items, int len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (items[i] > items[j]) {
        int tmp  = items[i];
        items[i] = items[j];
        items[j] = tmp;
      }
    }
  }
}

static void pairwise_swap_tmp_reused(int *items, int len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (items[i] > items[j]) {
        int tmp  = items[i];
        items[i] = items[j];
        items[j] = tmp;
        printf("tmp=%d\n", tmp);
      }
    }
  }
}

static void nested_self_swap(int *items, int len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (items[i] > 0) {
        int tmp  = items[j];
        items[j] = items[j];
        items[j] = tmp;
      }
    }
  }
}

int main(void) {
  int a[5] = {5, 3, 4, 1, 2};
  pairwise_swap(a, 5);
  printf("%d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);

  int b[3] = {3, 1, 2};
  pairwise_swap_tmp_reused(b, 3);
  printf("%d %d %d\n", b[0], b[1], b[2]);

  int c[3] = {7, 8, 9};
  nested_self_swap(c, 3);
  printf("%d %d %d\n", c[0], c[1], c[2]);
  return 0;
}

// REWRITES-LABEL: {{^}}fn pairwise_swap_tmp_reused(
// REWRITES-NOT: .swap(
// REWRITES: {{^}}}
// REWRITES-LABEL: {{^}}fn nested_self_swap(
// REWRITES-NOT: .swap(
// REWRITES: {{^}}}

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
// COMMON-LOWERING-NEXT:     let mut b: [i32; 3] = [0; 3];
// COMMON-LOWERING-NEXT:     let mut c: [i32; 3] = [0; 3];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 5] = [5, 3, 4, 1, 2];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     pairwise_swap({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 3] = [3, 1, 2];
// COMMON-LOWERING-NEXT:     b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = b.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     pairwise_swap_tmp_reused({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = b[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = b[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = b[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 3] = [7, 8, 9];
// COMMON-LOWERING-NEXT:     c = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = c.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     nested_self_swap({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn pairwise_swap({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let mut items: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut len: i32 = 0;
// COMMON-LOWERING-NEXT:     items = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     len = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let mut j: i32 = 0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                     j = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     loop {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                             break;
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                         {
// COMMON-LOWERING-NEXT:                             {
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     unsafe {
// COMMON-LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     unsafe {
// COMMON-LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                 }
// COMMON-LOWERING-NEXT:                             }
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                         j = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn pairwise_swap_tmp_reused({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let mut items: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut len: i32 = 0;
// COMMON-LOWERING-NEXT:     items = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     len = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let mut j: i32 = 0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                     j = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     loop {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                             break;
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                         {
// COMMON-LOWERING-NEXT:                             {
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     unsafe {
// COMMON-LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     unsafe {
// COMMON-LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                                         unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                                 }
// COMMON-LOWERING-NEXT:                             }
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                         j = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn nested_self_swap({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let mut items: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut len: i32 = 0;
// COMMON-LOWERING-NEXT:     items = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     len = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let mut j: i32 = 0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                     j = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     loop {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                             break;
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                         {
// COMMON-LOWERING-NEXT:                             {
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     unsafe {
// COMMON-LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                                     unsafe {
// COMMON-LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                 }
// COMMON-LOWERING-NEXT:                             }
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                         j = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-X86_64-GNU-NEXT:     *a = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                                     let {{__v[0-9]+}}: *mut i8 = b"tmp=%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut a: [i32; 5] = [0; 5];
// LOWERING-AARCH64-GNU-NEXT:     a = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                                     let {{__v[0-9]+}}: *mut u8 = b"tmp=%d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT:     let mut b: [i32; 3] = [0; 3];
// COMMON-REWRITES-NEXT:     let mut c: [i32; 3] = [0; 3];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     pairwise_swap(unsafe { std::slice::from_raw_parts_mut({{__v[0-9]+}} as *mut i32, (5 as i32) as usize) });
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d %d %d %d\n".as_ptr(), a[0], a[1], a[2], a[3], a[4]) };
// COMMON-REWRITES-NEXT:     b = [3, 1, 2];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = b.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     pairwise_swap_tmp_reused(unsafe {
// COMMON-REWRITES-NEXT:         std::slice::from_raw_parts_mut({{__v[0-9]+}} as *mut i32, (3 as i32) as usize)
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), b[0], b[1], b[2]) };
// COMMON-REWRITES-NEXT:     c = [7, 8, 9];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = c.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     nested_self_swap(unsafe {
// COMMON-REWRITES-NEXT:         std::slice::from_raw_parts_mut({{__v[0-9]+}} as *mut i32, (3 as i32) as usize)
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), c[0], c[1], c[2]) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn pairwise_swap({{arg[0-9]+}}: &mut [i32]) {
// COMMON-REWRITES-NEXT:     let mut items: *mut i32 = {{arg[0-9]+}}.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// COMMON-REWRITES-NEXT:     for i in 0..len {
// COMMON-REWRITES-NEXT:         for j in 0..len {
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:             if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-NEXT:                 unsafe {
// COMMON-REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// COMMON-REWRITES-NEXT:                 unsafe {
// COMMON-REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn pairwise_swap_tmp_reused({{arg[0-9]+}}: &mut [i32]) {
// COMMON-REWRITES-NEXT:     let mut items: *mut i32 = {{arg[0-9]+}}.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// COMMON-REWRITES-NEXT:     for i in 0..len {
// COMMON-REWRITES-NEXT:         for j in 0..len {
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:             if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-NEXT:                 unsafe {
// COMMON-REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// COMMON-REWRITES-NEXT:                 unsafe {
// COMMON-REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:                 unsafe { printf(c"tmp=%d\n".as_ptr(), {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn nested_self_swap({{arg[0-9]+}}: &mut [i32]) {
// COMMON-REWRITES-NEXT:     let mut items: *mut i32 = {{arg[0-9]+}}.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// COMMON-REWRITES-NEXT:     for i in 0..len {
// COMMON-REWRITES-NEXT:         for j in 0..len {
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: bool = (unsafe { *{{__v[0-9]+}} }) > 0;
// COMMON-REWRITES-NEXT:             if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// COMMON-REWRITES-NEXT:                 unsafe {
// COMMON-REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// COMMON-REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// COMMON-REWRITES-NEXT:                 unsafe {
// COMMON-REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-X86_64-GNU-NEXT:     *a = [5, 3, 4, 1, 2];
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut a: [i32; 5] = [5, 3, 4, 1, 2];
// SLATE-FILECHECK-END rewrites-aarch64-gnu
