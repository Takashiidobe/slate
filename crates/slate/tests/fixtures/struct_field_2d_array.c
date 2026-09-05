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
// COMMON-LOWERING-NEXT: struct cube {
// COMMON-LOWERING-NEXT:     v: {{\[\[}}[i32; 4]; 3]; 2],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct table {
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut t: table = table { rows: {{\[\[}}0; 3]; 4] };
// COMMON-LOWERING-NEXT:     let mut c: cube = cube {
// COMMON-LOWERING-NEXT:         v: {{\[\[}}[0; 4]; 3]; 2],
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     fill(std::ptr::addr_of_mut!(t));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     fill_via_ptr(std::ptr::addr_of_mut!(t), {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     fill_cube(std::ptr::addr_of_mut!(c));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c.v[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c.v[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum_cube_via_ptr(std::ptr::addr_of_mut!(c));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn fill({{arg[0-9]+}}: *mut table) {
// COMMON-LOWERING-NEXT:     let mut t: *mut table = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     t = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 97;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut table = t;
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     (*{{__v[0-9]+}}).rows[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 48;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut table = t;
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     (*{{__v[0-9]+}}).rows[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut table = t;
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     (*{{__v[0-9]+}}).rows[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
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
// COMMON-LOWERING-NEXT: fn fill_via_ptr({{arg[0-9]+}}: *mut table, {{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         (*{{__v[0-9]+}})[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn fill_cube({{arg[0-9]+}}: *mut cube) {
// COMMON-LOWERING-NEXT:     let mut c: *mut cube = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     c = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 2;
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
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                             break;
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                         {
// COMMON-LOWERING-NEXT:                             {
// COMMON-LOWERING-NEXT:                                 let mut k: i32 = 0;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                                 k = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                 loop {
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = k;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                     if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                                         break;
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                     {
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = 100;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = k;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = k;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: *mut cube = c;
// COMMON-LOWERING-NEXT:                                         unsafe {
// COMMON-LOWERING-NEXT:                                             (*{{__v[0-9]+}}).v[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)]
// COMMON-LOWERING-NEXT:                                                 [({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                         }
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = k;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                                     k = {{__v[0-9]+}};
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
// COMMON-LOWERING-NEXT: fn sum_cube_via_ptr({{arg[0-9]+}}: *mut cube) -> i32 {
// COMMON-LOWERING-NEXT:     let mut plane: *mut [i32; 4] = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut [i32; 4] =
// COMMON-LOWERING-NEXT:         (unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).v[({{__v[0-9]+}} as usize)]) }) as *mut [i32; 4];
// COMMON-LOWERING-NEXT:     plane = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut j: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         j = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let mut k: i32 = 0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                     k = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     loop {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = k;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                             break;
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                         {
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = k;
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: *mut [i32; 4] = plane;
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: *mut [i32; 4] = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: *mut [i32; 4] = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}})[({{__v[0-9]+}} as usize)] };
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                             total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = k;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                         k = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             j = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     rows: {{\[\[}}i8; 3]; 4],
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(t.rows[({{__v[0-9]+}} as usize)]) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: i8 = {{__v[0-9]+}} as i8;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: i8 = {{__v[0-9]+}} as i8;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: i8 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i8; 3] = (unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).rows) }) as *mut [i8; 3];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 88;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i8; 3] = unsafe { {{__v[0-9]+}}.offset({{arg[0-9]+}} as isize) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i8; 3] = unsafe { {{__v[0-9]+}}.add(0) };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     rows: {{\[\[}}u8; 3]; 4],
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = b"%s\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(t.rows[({{__v[0-9]+}} as usize)]) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: u8 = 0;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [u8; 3] = (unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).rows) }) as *mut [u8; 3];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 88;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [u8; 3] = unsafe { {{__v[0-9]+}}.offset({{arg[0-9]+}} as isize) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [u8; 3] = unsafe { {{__v[0-9]+}}.add(0) };
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
// COMMON-REWRITES-NEXT: struct cube {
// COMMON-REWRITES-NEXT:     v: {{\[\[}}[i32; 4]; 3]; 2],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct table {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut t: table = table { rows: {{\[\[}}0; 3]; 4] };
// COMMON-REWRITES-NEXT:     let mut c: cube = cube {
// COMMON-REWRITES-NEXT:         v: {{\[\[}}[0; 4]; 3]; 2],
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     fill(unsafe { &mut (*std::ptr::addr_of_mut!(t)) });
// COMMON-REWRITES-NEXT:     fill_via_ptr(unsafe { &mut (*std::ptr::addr_of_mut!(t)) }, 2);
// COMMON-REWRITES-NEXT:     for i in 0..4 {
// COMMON-REWRITES-NEXT:         unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     fill_cube(unsafe { &mut (*std::ptr::addr_of_mut!(c)) });
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             c.v[0][0][0],
// COMMON-REWRITES-NEXT:             c.v[1][2][3],
// COMMON-REWRITES-NEXT:             sum_cube_via_ptr(unsafe { &mut (*std::ptr::addr_of_mut!(c)) }),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn fill({{arg[0-9]+}}: &mut table) {
// COMMON-REWRITES-NEXT:     for i in 0..4 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i64 = 0;
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i64 = 1;
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i64 = 2;
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             (*{{arg[0-9]+}}).rows[((i as i64) as usize)][({{__v[0-9]+}} as usize)] = 0;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn fill_via_ptr({{arg[0-9]+}}: &mut table, {{arg[0-9]+}}: i32) {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         (*{{__v[0-9]+}})[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn fill_cube({{arg[0-9]+}}: &mut cube) {
// COMMON-REWRITES-NEXT:     for i in 0..2 {
// COMMON-REWRITES-NEXT:         for j in 0..3 {
// COMMON-REWRITES-NEXT:             for k in 0..4 {
// COMMON-REWRITES-NEXT:                 unsafe {
// COMMON-REWRITES-NEXT:                     (*{{arg[0-9]+}}).v[((i as i64) as usize)][((j as i64) as usize)]
// COMMON-REWRITES-NEXT:                         [((k as i64) as usize)] = i * 100 + j * 10 + k;
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn sum_cube_via_ptr({{arg[0-9]+}}: &mut cube) -> i32 {
// COMMON-REWRITES-NEXT:     let mut plane: *mut [i32; 4] = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut [i32; 4] =
// COMMON-REWRITES-NEXT:         (unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut cube)).v[1]) }) as *mut [i32; 4];
// COMMON-REWRITES-NEXT:     plane = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     for j in 0..3 {
// COMMON-REWRITES-NEXT:         for k in 0..4 {
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: i64 = k as i64;
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut [i32; 4] = plane;
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut [i32; 4] = unsafe { {{__v[0-9]+}}.offset(j as isize) };
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: *mut [i32; 4] = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-REWRITES-NEXT:             total += unsafe { (*{{__v[0-9]+}})[({{__v[0-9]+}} as usize)] };
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     rows: {{\[\[}}i8; 3]; 4],
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = c"%s\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(t.rows[((i as i64) as usize)]) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:             (*{{arg[0-9]+}}).rows[((i as i64) as usize)][({{__v[0-9]+}} as usize)] = (97 + i) as i8;
// REWRITES-X86_64-GNU-NEXT:             (*{{arg[0-9]+}}).rows[((i as i64) as usize)][({{__v[0-9]+}} as usize)] = (48 + i) as i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i8; 3] =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut table)).rows) }) as *mut [i8; 3];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 88;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i8; 3] = unsafe { {{__v[0-9]+}}.offset({{arg[0-9]+}} as isize) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i8; 3] = unsafe { {{__v[0-9]+}}.add(0) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     rows: {{\[\[}}u8; 3]; 4],
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = c"%s\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(t.rows[((i as i64) as usize)]) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:             (*{{arg[0-9]+}}).rows[((i as i64) as usize)][({{__v[0-9]+}} as usize)] = (97 + i) as u8;
// REWRITES-AARCH64-GNU-NEXT:             (*{{arg[0-9]+}}).rows[((i as i64) as usize)][({{__v[0-9]+}} as usize)] = (48 + i) as u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [u8; 3] =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut table)).rows) }) as *mut [u8; 3];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 88;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [u8; 3] = unsafe { {{__v[0-9]+}}.offset({{arg[0-9]+}} as isize) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [u8; 3] = unsafe { {{__v[0-9]+}}.add(0) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
