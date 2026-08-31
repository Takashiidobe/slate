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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pairwise_swap({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut items: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut len: i32 = 0;
// LOWERING-NEXT:     items = {{arg[0-9]+}};
// LOWERING-NEXT:     len = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = len;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut j: i32 = 0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                     j = {{_v[0-9]+}};
// LOWERING-NEXT:                     loop {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = len;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                         if !{{_v[0-9]+}} {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             {
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:                                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                         j = {{_v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
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
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = len;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut j: i32 = 0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                     j = {{_v[0-9]+}};
// LOWERING-NEXT:                     loop {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = len;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                         if !{{_v[0-9]+}} {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             {
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:                                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i8 = b"tmp=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                         j = {{_v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
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
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = len;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut j: i32 = 0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                     j = {{_v[0-9]+}};
// LOWERING-NEXT:                     loop {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = len;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                         if !{{_v[0-9]+}} {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             {
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:                                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                                     unsafe {
// LOWERING-NEXT:                                         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                         j = {{_v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-NEXT:     let mut b: [i32; 3] = [0; 3];
// LOWERING-NEXT:     let mut c: [i32; 3] = [0; 3];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     *a = [5, 3, 4, 1, 2];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     pairwise_swap({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = a[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = a[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = a[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = a[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = a[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     b = [3, 1, 2];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = b.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     pairwise_swap_tmp_reused({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = b[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = b[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = b[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     c = [7, 8, 9];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = c.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     nested_self_swap({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-LABEL: {{^}}fn pairwise_swap_tmp_reused(
// REWRITES-NOT: .swap(
// REWRITES: {{^}}}
// REWRITES-LABEL: {{^}}fn nested_self_swap(
// REWRITES-NOT: .swap(
// REWRITES: {{^}}}
