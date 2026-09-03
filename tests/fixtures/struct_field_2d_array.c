#include <stdio.h>

struct table {
  char rows[4][3];
};

struct cube {
  int v[2][3][4];
};

static void fill(struct table *t) {
  for (int i = 0; i < 4; i++) {
    t->rows[i][0] = (char)('a' + i);
    t->rows[i][1] = (char)('0' + i);
    t->rows[i][2] = '\0';
  }
}

static void fill_via_ptr(struct table *t, int i) {
  char (*row)[3]  = t->rows;
  (row + i)[0][0] = 'X';
}

static void fill_cube(struct cube *c) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 4; k++) {
        c->v[i][j][k] = i * 100 + j * 10 + k;
      }
    }
  }
}

static int sum_cube_via_ptr(struct cube *c) {
  int (*plane)[4] = c->v[1];
  int total       = 0;
  for (int j = 0; j < 3; j++) {
    for (int k = 0; k < 4; k++) {
      total += (plane + j)[0][k];
    }
  }
  return total;
}

int main(void) {
  struct table t;
  fill(&t);
  fill_via_ptr(&t, 2);

  for (int i = 0; i < 4; i++) {
    printf("%s\n", t.rows[i]);
  }

  struct cube c;
  fill_cube(&c);
  printf("%d %d %d\n", c.v[0][0][0], c.v[1][2][3], sum_cube_via_ptr(&c));
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
// LOWERING-NEXT: struct cube {
// LOWERING-NEXT:     v: {{\[\[}}[i32; 4]; 3]; 2],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct table {
// LOWERING-NEXT:     rows: {{\[\[}}i8; 3]; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fill({{arg[0-9]+}}: *mut table) {
// LOWERING-NEXT:     let mut t: *mut table = std::ptr::null_mut();
// LOWERING-NEXT:     t = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 97;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i8 = {{_v[0-9]+}} as i8;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut table = t;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     (*{{_v[0-9]+}}).rows[({{_v[0-9]+}} as usize)][({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 48;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i8 = {{_v[0-9]+}} as i8;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut table = t;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     (*{{_v[0-9]+}}).rows[({{_v[0-9]+}} as usize)][({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i8 = 0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut table = t;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     (*{{_v[0-9]+}}).rows[({{_v[0-9]+}} as usize)][({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
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
// LOWERING-NEXT: fn fill_via_ptr({{arg[0-9]+}}: *mut table, {{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut [i8; 3] = (unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).rows) }) as *mut [i8; 3];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 88;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut [i8; 3] = unsafe { {{_v[0-9]+}}.offset({{arg[0-9]+}} as isize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut [i8; 3] = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{_v[0-9]+}})[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fill_cube({{arg[0-9]+}}: *mut cube) {
// LOWERING-NEXT:     let mut c: *mut cube = std::ptr::null_mut();
// LOWERING-NEXT:     c = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 2;
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
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                         if !{{_v[0-9]+}} {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             {
// LOWERING-NEXT:                                 let mut k: i32 = 0;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                                 k = {{_v[0-9]+}};
// LOWERING-NEXT:                                 loop {
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = k;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                                     if !{{_v[0-9]+}} {
// LOWERING-NEXT:                                         break;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     {
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = k;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = k;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: *mut cube = c;
// LOWERING-NEXT:                                         unsafe {
// LOWERING-NEXT:                                             (*{{_v[0-9]+}}).v[({{_v[0-9]+}} as usize)][({{_v[0-9]+}} as usize)]
// LOWERING-NEXT:                                                 [({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:                                         }
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = k;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                                     k = {{_v[0-9]+}};
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
// LOWERING-NEXT: fn sum_cube_via_ptr({{arg[0-9]+}}: *mut cube) -> i32 {
// LOWERING-NEXT:     let mut plane: *mut [i32; 4] = std::ptr::null_mut();
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut [i32; 4] =
// LOWERING-NEXT:         (unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).v[({{_v[0-9]+}} as usize)]) }) as *mut [i32; 4];
// LOWERING-NEXT:     plane = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut j: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         j = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut k: i32 = 0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                     k = {{_v[0-9]+}};
// LOWERING-NEXT:                     loop {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = k;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                         if !{{_v[0-9]+}} {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = k;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: *mut [i32; 4] = plane;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: *mut [i32; 4] = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                             let {{_v[0-9]+}}: *mut [i32; 4] = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}})[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                             total = {{_v[0-9]+}};
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = k;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                         k = {{_v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             j = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut t: table = table { rows: {{\[\[}}0; 3]; 4] };
// LOWERING-NEXT:     let mut c: cube = cube {
// LOWERING-NEXT:         v: {{\[\[}}[0; 4]; 3]; 2],
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     fill(std::ptr::addr_of_mut!(t));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     fill_via_ptr(std::ptr::addr_of_mut!(t), {{_v[0-9]+}});
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(t.rows[({{_v[0-9]+}} as usize)]) as *mut i8;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     fill_cube(std::ptr::addr_of_mut!(c));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c.v[({{_v[0-9]+}} as usize)][({{_v[0-9]+}} as usize)][({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c.v[({{_v[0-9]+}} as usize)][({{_v[0-9]+}} as usize)][({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sum_cube_via_ptr(std::ptr::addr_of_mut!(c));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: struct cube {
// REWRITES-NEXT:     v: {{\[\[}}[i32; 4]; 3]; 2],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct table {
// REWRITES-NEXT:     rows: {{\[\[}}i8; 3]; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill({{arg[0-9]+}}: &mut table) {
// REWRITES-NEXT:     for i in 0..4 {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             (*{{arg[0-9]+}}).rows[((i as i64) as usize)][({{_v[0-9]+}} as usize)] = (97 + i) as i8;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             (*{{arg[0-9]+}}).rows[((i as i64) as usize)][({{_v[0-9]+}} as usize)] = (48 + i) as i8;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             (*{{arg[0-9]+}}).rows[((i as i64) as usize)][({{_v[0-9]+}} as usize)] = 0;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill_via_ptr({{arg[0-9]+}}: &mut table, {{arg[0-9]+}}: i32) {
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut [i8; 3] =
// REWRITES-NEXT:         (unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut table)).rows) }) as *mut [i8; 3];
// REWRITES-NEXT:     let {{_v[0-9]+}}: i8 = 88;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut [i8; 3] = unsafe { {{_v[0-9]+}}.offset({{arg[0-9]+}} as isize) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut [i8; 3] = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{_v[0-9]+}})[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill_cube({{arg[0-9]+}}: &mut cube) {
// REWRITES-NEXT:     for i in 0..2 {
// REWRITES-NEXT:         for j in 0..3 {
// REWRITES-NEXT:             for k in 0..4 {
// REWRITES-NEXT:                 unsafe {
// REWRITES-NEXT:                     (*{{arg[0-9]+}}).v[((i as i64) as usize)][((j as i64) as usize)]
// REWRITES-NEXT:                         [((k as i64) as usize)] = i * 100 + j * 10 + k;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum_cube_via_ptr({{arg[0-9]+}}: &mut cube) -> i32 {
// REWRITES-NEXT:     let mut plane: *mut [i32; 4] = std::ptr::null_mut();
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut [i32; 4] =
// REWRITES-NEXT:         (unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut cube)).v[1]) }) as *mut [i32; 4];
// REWRITES-NEXT:     plane = {{_v[0-9]+}};
// REWRITES-NEXT:     for j in 0..3 {
// REWRITES-NEXT:         for k in 0..4 {
// REWRITES-NEXT:             let {{_v[0-9]+}}: i64 = k as i64;
// REWRITES-NEXT:             let {{_v[0-9]+}}: *mut [i32; 4] = plane;
// REWRITES-NEXT:             let {{_v[0-9]+}}: *mut [i32; 4] = unsafe { {{_v[0-9]+}}.offset(j as isize) };
// REWRITES-NEXT:             let {{_v[0-9]+}}: *mut [i32; 4] = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:             total += unsafe { (*{{_v[0-9]+}})[({{_v[0-9]+}} as usize)] };
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     total
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut t: table = table { rows: {{\[\[}}0; 3]; 4] };
// REWRITES-NEXT:     let mut c: cube = cube {
// REWRITES-NEXT:         v: {{\[\[}}[0; 4]; 3]; 2],
// REWRITES-NEXT:     };
// REWRITES-NEXT:     fill(unsafe { &mut (*std::ptr::addr_of_mut!(t)) });
// REWRITES-NEXT:     fill_via_ptr(unsafe { &mut (*std::ptr::addr_of_mut!(t)) }, 2);
// REWRITES-NEXT:     for i in 0..4 {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = c"%s\n".as_ptr() as *mut i8;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(t.rows[((i as i64) as usize)]) as *mut i8;
// REWRITES-NEXT:         unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     fill_cube(unsafe { &mut (*std::ptr::addr_of_mut!(c)) });
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// REWRITES-NEXT:             c.v[0][0][0],
// REWRITES-NEXT:             c.v[1][2][3],
// REWRITES-NEXT:             sum_cube_via_ptr(unsafe { &mut (*std::ptr::addr_of_mut!(c)) }),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
