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
// LOWERING-X86_64-GNU-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-AARCH64-GNU-NEXT:     let mut a: [i32; 5] = [0; 5];
// LOWERING-NEXT:     let mut b: [i32; 3] = [0; 3];
// LOWERING-NEXT:     let mut c: [i32; 3] = [0; 3];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 5] = [5, 3, 4, 1, 2];
// LOWERING-X86_64-GNU-NEXT:     *a = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     a = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     pairwise_swap({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 3] = [3, 1, 2];
// LOWERING-NEXT:     b = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = b.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     pairwise_swap_tmp_reused({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = b[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = b[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = b[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 3] = [7, 8, 9];
// LOWERING-NEXT:     c = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = c.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     nested_self_swap({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pairwise_swap({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
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
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut j: i32 = 0;
// LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                     j = {{__v[0-9]+}};
// LOWERING-NEXT:                     loop {
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = len;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:                         if !{{__v[0-9]+}} {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             {
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:                                 if {{__v[0-9]+}} {
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:                         j = {{__v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pairwise_swap_tmp_reused({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
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
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut j: i32 = 0;
// LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                     j = {{__v[0-9]+}};
// LOWERING-NEXT:                     loop {
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = len;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:                         if !{{__v[0-9]+}} {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             {
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:                                 if {{__v[0-9]+}} {
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-X86_64-GNU-NEXT:                                     let {{__v[0-9]+}}: *mut i8 = b"tmp=%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:                                     let {{__v[0-9]+}}: *mut u8 = b"tmp=%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 =
// LOWERING-NEXT:                                         unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:                         j = {{__v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn nested_self_swap({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
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
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut j: i32 = 0;
// LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                     j = {{__v[0-9]+}};
// LOWERING-NEXT:                     loop {
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = len;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:                         if !{{__v[0-9]+}} {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             {
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:                                 if {{__v[0-9]+}} {
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:                         j = {{__v[0-9]+}};
// LOWERING-NEXT:                     }
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
// REWRITES-X86_64-GNU-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-AARCH64-GNU-NEXT:     let mut a: [i32; 5] = [5, 3, 4, 1, 2];
// REWRITES-NEXT:     let mut b: [i32; 3] = [0; 3];
// REWRITES-NEXT:     let mut c: [i32; 3] = [0; 3];
// REWRITES-X86_64-GNU-NEXT:     *a = [5, 3, 4, 1, 2];
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     pairwise_swap(unsafe { std::slice::from_raw_parts_mut({{__v[0-9]+}} as *mut i32, (5 as i32) as usize) });
// REWRITES-NEXT:     unsafe { printf(c"%d %d %d %d %d\n".as_ptr(), a[0], a[1], a[2], a[3], a[4]) };
// REWRITES-NEXT:     b = [3, 1, 2];
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = b.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     pairwise_swap_tmp_reused(unsafe {
// REWRITES-NEXT:         std::slice::from_raw_parts_mut({{__v[0-9]+}} as *mut i32, (3 as i32) as usize)
// REWRITES-NEXT:     });
// REWRITES-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), b[0], b[1], b[2]) };
// REWRITES-NEXT:     c = [7, 8, 9];
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = c.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     nested_self_swap(unsafe {
// REWRITES-NEXT:         std::slice::from_raw_parts_mut({{__v[0-9]+}} as *mut i32, (3 as i32) as usize)
// REWRITES-NEXT:     });
// REWRITES-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), c[0], c[1], c[2]) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn pairwise_swap({{arg[0-9]+}}: &mut [i32]) {
// REWRITES-NEXT:     let mut items: *mut i32 = {{arg[0-9]+}}.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-NEXT:     for i in 0..len {
// REWRITES-NEXT:         for j in 0..len {
// REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// REWRITES-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:             if {{__v[0-9]+}} {
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:                 unsafe {
// REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// REWRITES-NEXT:                 unsafe {
// REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:                 }
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn pairwise_swap_tmp_reused({{arg[0-9]+}}: &mut [i32]) {
// REWRITES-NEXT:     let mut items: *mut i32 = {{arg[0-9]+}}.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-NEXT:     for i in 0..len {
// REWRITES-NEXT:         for j in 0..len {
// REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// REWRITES-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:             if {{__v[0-9]+}} {
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:                 unsafe {
// REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// REWRITES-NEXT:                 unsafe {
// REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 unsafe { printf(c"tmp=%d\n".as_ptr(), {{__v[0-9]+}}) };
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn nested_self_swap({{arg[0-9]+}}: &mut [i32]) {
// REWRITES-NEXT:     let mut items: *mut i32 = {{arg[0-9]+}}.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-NEXT:     for i in 0..len {
// REWRITES-NEXT:         for j in 0..len {
// REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:             let {{__v[0-9]+}}: bool = (unsafe { *{{__v[0-9]+}} }) > 0;
// REWRITES-NEXT:             if {{__v[0-9]+}} {
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// REWRITES-NEXT:                 unsafe {
// REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((j as i64) as isize) };
// REWRITES-NEXT:                 unsafe {
// REWRITES-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:                 }
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
