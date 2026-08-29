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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct cube {
// LOWERING-NEXT:     v: {{\[\[}}[i32; 4]; 3]; 2],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct table {
// LOWERING-NEXT:     rows: {{\[\[}}i8; 3]; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fill(arg4: *mut table) {
// LOWERING-NEXT:     let mut t: *mut table = std::ptr::null_mut();
// LOWERING-NEXT:     t = arg4;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v0: i32 = 0;
// LOWERING-NEXT:         i = _v0;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v1: i32 = i;
// LOWERING-NEXT:             let _v2: i32 = 4;
// LOWERING-NEXT:             let _v3: bool = _v1 < _v2;
// LOWERING-NEXT:             if !_v3 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v4: i32 = 97;
// LOWERING-NEXT:                 let _v5: i32 = i;
// LOWERING-NEXT:                 let _v6: i32 = _v4 + _v5;
// LOWERING-NEXT:                 let _v7: i8 = _v6 as i8;
// LOWERING-NEXT:                 let _v8: i64 = 0;
// LOWERING-NEXT:                 let _v9: i32 = i;
// LOWERING-NEXT:                 let _v10: i64 = _v9 as i64;
// LOWERING-NEXT:                 let _v11: *mut table = t;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     (*_v11).rows[(_v10 as usize)][(_v8 as usize)] = _v7;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v12: i32 = 48;
// LOWERING-NEXT:                 let _v13: i32 = i;
// LOWERING-NEXT:                 let _v14: i32 = _v12 + _v13;
// LOWERING-NEXT:                 let _v15: i8 = _v14 as i8;
// LOWERING-NEXT:                 let _v16: i64 = 1;
// LOWERING-NEXT:                 let _v17: i32 = i;
// LOWERING-NEXT:                 let _v18: i64 = _v17 as i64;
// LOWERING-NEXT:                 let _v19: *mut table = t;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     (*_v19).rows[(_v18 as usize)][(_v16 as usize)] = _v15;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v20: i8 = 0;
// LOWERING-NEXT:                 let _v21: i64 = 2;
// LOWERING-NEXT:                 let _v22: i32 = i;
// LOWERING-NEXT:                 let _v23: i64 = _v22 as i64;
// LOWERING-NEXT:                 let _v24: *mut table = t;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     (*_v24).rows[(_v23 as usize)][(_v21 as usize)] = _v20;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v25: i32 = i;
// LOWERING-NEXT:             let _v26: i32 = _v25 + 1;
// LOWERING-NEXT:             i = _v26;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fill_via_ptr(arg2: *mut table, arg3: i32) {
// LOWERING-NEXT:     let mut t: *mut table = std::ptr::null_mut();
// LOWERING-NEXT:     let mut i: i32 = 0;
// LOWERING-NEXT:     let mut row: *mut [i8; 3] = std::ptr::null_mut();
// LOWERING-NEXT:     t = arg2;
// LOWERING-NEXT:     i = arg3;
// LOWERING-NEXT:     let _v0: *mut table = t;
// LOWERING-NEXT:     let _v1: *mut [i8; 3] = (unsafe { std::ptr::addr_of_mut!((*_v0).rows) }) as *mut [i8; 3];
// LOWERING-NEXT:     row = _v1;
// LOWERING-NEXT:     let _v2: i8 = 88;
// LOWERING-NEXT:     let _v3: i64 = 0;
// LOWERING-NEXT:     let _v4: i64 = 0;
// LOWERING-NEXT:     let _v5: *mut [i8; 3] = row;
// LOWERING-NEXT:     let _v6: i32 = i;
// LOWERING-NEXT:     let _v7: *mut [i8; 3] = unsafe { _v5.offset(_v6 as isize) };
// LOWERING-NEXT:     let _v8: *mut [i8; 3] = unsafe { _v7.add(0) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v8)[(_v3 as usize)] = _v2;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fill_cube(arg1: *mut cube) {
// LOWERING-NEXT:     let mut c: *mut cube = std::ptr::null_mut();
// LOWERING-NEXT:     c = arg1;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v0: i32 = 0;
// LOWERING-NEXT:         i = _v0;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v1: i32 = i;
// LOWERING-NEXT:             let _v2: i32 = 2;
// LOWERING-NEXT:             let _v3: bool = _v1 < _v2;
// LOWERING-NEXT:             if !_v3 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut j: i32 = 0;
// LOWERING-NEXT:                     let _v4: i32 = 0;
// LOWERING-NEXT:                     j = _v4;
// LOWERING-NEXT:                     loop {
// LOWERING-NEXT:                         let _v5: i32 = j;
// LOWERING-NEXT:                         let _v6: i32 = 3;
// LOWERING-NEXT:                         let _v7: bool = _v5 < _v6;
// LOWERING-NEXT:                         if !_v7 {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             {
// LOWERING-NEXT:                                 let mut k: i32 = 0;
// LOWERING-NEXT:                                 let _v8: i32 = 0;
// LOWERING-NEXT:                                 k = _v8;
// LOWERING-NEXT:                                 loop {
// LOWERING-NEXT:                                     let _v9: i32 = k;
// LOWERING-NEXT:                                     let _v10: i32 = 4;
// LOWERING-NEXT:                                     let _v11: bool = _v9 < _v10;
// LOWERING-NEXT:                                     if !_v11 {
// LOWERING-NEXT:                                         break;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     {
// LOWERING-NEXT:                                         let _v12: i32 = i;
// LOWERING-NEXT:                                         let _v13: i32 = 100;
// LOWERING-NEXT:                                         let _v14: i32 = _v12 * _v13;
// LOWERING-NEXT:                                         let _v15: i32 = j;
// LOWERING-NEXT:                                         let _v16: i32 = 10;
// LOWERING-NEXT:                                         let _v17: i32 = _v15 * _v16;
// LOWERING-NEXT:                                         let _v18: i32 = _v14 + _v17;
// LOWERING-NEXT:                                         let _v19: i32 = k;
// LOWERING-NEXT:                                         let _v20: i32 = _v18 + _v19;
// LOWERING-NEXT:                                         let _v21: i32 = k;
// LOWERING-NEXT:                                         let _v22: i64 = _v21 as i64;
// LOWERING-NEXT:                                         let _v23: i32 = j;
// LOWERING-NEXT:                                         let _v24: i64 = _v23 as i64;
// LOWERING-NEXT:                                         let _v25: i32 = i;
// LOWERING-NEXT:                                         let _v26: i64 = _v25 as i64;
// LOWERING-NEXT:                                         let _v27: *mut cube = c;
// LOWERING-NEXT:                                         unsafe {
// LOWERING-NEXT:                                             (*_v27).v[(_v26 as usize)][(_v24 as usize)][(_v22 as usize)] = _v20;
// LOWERING-NEXT:                                         }
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     let _v28: i32 = k;
// LOWERING-NEXT:                                     let _v29: i32 = _v28 + 1;
// LOWERING-NEXT:                                     k = _v29;
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let _v30: i32 = j;
// LOWERING-NEXT:                         let _v31: i32 = _v30 + 1;
// LOWERING-NEXT:                         j = _v31;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v32: i32 = i;
// LOWERING-NEXT:             let _v33: i32 = _v32 + 1;
// LOWERING-NEXT:             i = _v33;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sum_cube_via_ptr(arg0: *mut cube) -> i32 {
// LOWERING-NEXT:     let mut c: *mut cube = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut plane: *mut [i32; 4] = std::ptr::null_mut();
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     c = arg0;
// LOWERING-NEXT:     let _v0: i64 = 1;
// LOWERING-NEXT:     let _v1: *mut cube = c;
// LOWERING-NEXT:     let _v2: *mut [i32; 4] = (unsafe { std::ptr::addr_of_mut!((*_v1).v[(_v0 as usize)]) }) as *mut [i32; 4];
// LOWERING-NEXT:     plane = _v2;
// LOWERING-NEXT:     let _v3: i32 = 0;
// LOWERING-NEXT:     total = _v3;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut j: i32 = 0;
// LOWERING-NEXT:         let _v4: i32 = 0;
// LOWERING-NEXT:         j = _v4;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v5: i32 = j;
// LOWERING-NEXT:             let _v6: i32 = 3;
// LOWERING-NEXT:             let _v7: bool = _v5 < _v6;
// LOWERING-NEXT:             if !_v7 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut k: i32 = 0;
// LOWERING-NEXT:                     let _v8: i32 = 0;
// LOWERING-NEXT:                     k = _v8;
// LOWERING-NEXT:                     loop {
// LOWERING-NEXT:                         let _v9: i32 = k;
// LOWERING-NEXT:                         let _v10: i32 = 4;
// LOWERING-NEXT:                         let _v11: bool = _v9 < _v10;
// LOWERING-NEXT:                         if !_v11 {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             let _v12: i32 = k;
// LOWERING-NEXT:                             let _v13: i64 = _v12 as i64;
// LOWERING-NEXT:                             let _v14: i64 = 0;
// LOWERING-NEXT:                             let _v15: *mut [i32; 4] = plane;
// LOWERING-NEXT:                             let _v16: i32 = j;
// LOWERING-NEXT:                             let _v17: *mut [i32; 4] = unsafe { _v15.offset(_v16 as isize) };
// LOWERING-NEXT:                             let _v18: *mut [i32; 4] = unsafe { _v17.add(0) };
// LOWERING-NEXT:                             let _v19: i32 = unsafe { (*_v18)[(_v13 as usize)] };
// LOWERING-NEXT:                             let _v20: i32 = total;
// LOWERING-NEXT:                             let _v21: i32 = _v20 + _v19;
// LOWERING-NEXT:                             total = _v21;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let _v22: i32 = k;
// LOWERING-NEXT:                         let _v23: i32 = _v22 + 1;
// LOWERING-NEXT:                         k = _v23;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v24: i32 = j;
// LOWERING-NEXT:             let _v25: i32 = _v24 + 1;
// LOWERING-NEXT:             j = _v25;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v26: i32 = total;
// LOWERING-NEXT:     __retval = _v26;
// LOWERING-NEXT:     let _v27: i32 = __retval;
// LOWERING-NEXT:     return _v27;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut t: table = table { rows: {{\[\[}}0; 3]; 4] };
// LOWERING-NEXT:     let mut c: cube = cube { v: {{\[\[}}[0; 4]; 3]; 2] };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     fill(std::ptr::addr_of_mut!(t));
// LOWERING-NEXT:     let _v1: i32 = 2;
// LOWERING-NEXT:     fill_via_ptr(std::ptr::addr_of_mut!(t), _v1);
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v2: i32 = 0;
// LOWERING-NEXT:         i = _v2;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v3: i32 = i;
// LOWERING-NEXT:             let _v4: i32 = 4;
// LOWERING-NEXT:             let _v5: bool = _v3 < _v4;
// LOWERING-NEXT:             if !_v5 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v6: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v7: i32 = i;
// LOWERING-NEXT:                 let _v8: i64 = _v7 as i64;
// LOWERING-NEXT:                 let _v9: *mut i8 = std::ptr::addr_of_mut!(t.rows[(_v8 as usize)]) as *mut i8;
// LOWERING-NEXT:                 let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v11: i32 = i;
// LOWERING-NEXT:             let _v12: i32 = _v11 + 1;
// LOWERING-NEXT:             i = _v12;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     fill_cube(std::ptr::addr_of_mut!(c));
// LOWERING-NEXT:     let _v13: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: i64 = 0;
// LOWERING-NEXT:     let _v15: i64 = 0;
// LOWERING-NEXT:     let _v16: i64 = 0;
// LOWERING-NEXT:     let _v17: i32 = c.v[(_v16 as usize)][(_v15 as usize)][(_v14 as usize)];
// LOWERING-NEXT:     let _v18: i64 = 3;
// LOWERING-NEXT:     let _v19: i64 = 2;
// LOWERING-NEXT:     let _v20: i64 = 1;
// LOWERING-NEXT:     let _v21: i32 = c.v[(_v20 as usize)][(_v19 as usize)][(_v18 as usize)];
// LOWERING-NEXT:     let _v22: i32 = sum_cube_via_ptr(std::ptr::addr_of_mut!(c));
// LOWERING-NEXT:     let _v23: i32 = unsafe { printf(_v13 as *const i8, _v17, _v21, _v22) };
// LOWERING-NEXT:     let _v24: i32 = 0;
// LOWERING-NEXT:     __retval = _v24;
// LOWERING-NEXT:     let _v25: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v25 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct cube {
// REWRITES-NEXT:     v: {{\[\[}}[i32; 4]; 3]; 2],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct table {
// REWRITES-NEXT:     rows: {{\[\[}}i8; 3]; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill(arg4: &table) {
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v2: i32 = 4;
// REWRITES-NEXT:                     if !(i < _v2) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v4: i32 = 97;
// REWRITES-NEXT:                                     let _v8: i64 = 0;
// REWRITES-NEXT:                                     unsafe {
// REWRITES-NEXT:                                                         (*((arg4 as *const table) as *mut table)).rows[((i as i64) as usize)][(_v8 as usize)] = (_v4 + i) as i8;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     let _v12: i32 = 48;
// REWRITES-NEXT:                                     let _v16: i64 = 1;
// REWRITES-NEXT:                                     unsafe {
// REWRITES-NEXT:                                                         (*((arg4 as *const table) as *mut table)).rows[((i as i64) as usize)][(_v16 as usize)] = (_v12 + i) as i8;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     let _v21: i64 = 2;
// REWRITES-NEXT:                                     unsafe {
// REWRITES-NEXT:                                                         (*((arg4 as *const table) as *mut table)).rows[((i as i64) as usize)][(_v21 as usize)] = 0;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill_via_ptr(arg2: &table, arg3: i32) {
// REWRITES-NEXT: let mut i: i32 = arg3;
// REWRITES-NEXT: let mut row: *mut [i8; 3] = std::ptr::null_mut();
// REWRITES-NEXT: let _v1: *mut [i8; 3] = (unsafe { std::ptr::addr_of_mut!((*((arg2 as *const table) as *mut table)).rows) }) as *mut [i8; 3];
// REWRITES-NEXT: row = _v1;
// REWRITES-NEXT: let _v2: i8 = 88;
// REWRITES-NEXT: let _v3: i64 = 0;
// REWRITES-NEXT: let _v4: i64 = 0;
// REWRITES-NEXT: let _v5: *mut [i8; 3] = row;
// REWRITES-NEXT: let _v7: *mut [i8; 3] = unsafe { _v5.offset(i as isize) };
// REWRITES-NEXT: let _v8: *mut [i8; 3] = unsafe { _v7.add(0) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*_v8)[(_v3 as usize)] = _v2;
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill_cube(arg1: &cube) {
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v2: i32 = 2;
// REWRITES-NEXT:                     if !(i < _v2) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         let mut j: i32 = 0;
// REWRITES-NEXT:                                                         j = 0;
// REWRITES-NEXT:                                                         loop {
// REWRITES-NEXT:                                                                                 let _v6: i32 = 3;
// REWRITES-NEXT:                                                                                 if !(j < _v6) {
// REWRITES-NEXT:                                                                                                             break;
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                                                 {
// REWRITES-NEXT:                                                                                                             {
// REWRITES-NEXT:                                                                                                                                             let mut k: i32 = 0;
// REWRITES-NEXT:                                                                                                                                             k = 0;
// REWRITES-NEXT:                                                                                                                                             loop {
// REWRITES-NEXT:                                                                                                                                                                                 let _v10: i32 = 4;
// REWRITES-NEXT:                                                                                                                                                                                 if !(k < _v10) {
// REWRITES-NEXT:                                                                                                                                                                                                                         break;
// REWRITES-NEXT:                                                                                                                                                                                 }
// REWRITES-NEXT:                                                                                                                                                                                 {
// REWRITES-NEXT:                                                                                                                                                                                                                         let _v13: i32 = 100;
// REWRITES-NEXT:                                                                                                                                                                                                                         let _v16: i32 = 10;
// REWRITES-NEXT:                                                                                                                                                                                                                         unsafe {
// REWRITES-NEXT:                                                                                                                                                                                                                                                                     (*((arg1 as *const cube) as *mut cube)).v[((i as i64) as usize)][((j as i64) as usize)][((k as i64) as usize)] = i * _v13 + j * _v16 + k;
// REWRITES-NEXT:                                                                                                                                                                                                                         }
// REWRITES-NEXT:                                                                                                                                                                                 }
// REWRITES-NEXT:                                                                                                                                                                                 k = k + 1;
// REWRITES-NEXT:                                                                                                                                             }
// REWRITES-NEXT:                                                                                                             }
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                                                 j = j + 1;
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum_cube_via_ptr(arg0: &cube) -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut plane: *mut [i32; 4] = std::ptr::null_mut();
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: let _v0: i64 = 1;
// REWRITES-NEXT: let _v2: *mut [i32; 4] = (unsafe { std::ptr::addr_of_mut!((*((arg0 as *const cube) as *mut cube)).v[(_v0 as usize)]) }) as *mut [i32; 4];
// REWRITES-NEXT: plane = _v2;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut j: i32 = 0;
// REWRITES-NEXT:         j = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v6: i32 = 3;
// REWRITES-NEXT:                     if !(j < _v6) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         let mut k: i32 = 0;
// REWRITES-NEXT:                                                         k = 0;
// REWRITES-NEXT:                                                         loop {
// REWRITES-NEXT:                                                                                 let _v10: i32 = 4;
// REWRITES-NEXT:                                                                                 if !(k < _v10) {
// REWRITES-NEXT:                                                                                                             break;
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                                                 {
// REWRITES-NEXT:                                                                                                             let _v13: i64 = k as i64;
// REWRITES-NEXT:                                                                                                             let _v14: i64 = 0;
// REWRITES-NEXT:                                                                                                             let _v15: *mut [i32; 4] = plane;
// REWRITES-NEXT:                                                                                                             let _v17: *mut [i32; 4] = unsafe { _v15.offset(j as isize) };
// REWRITES-NEXT:                                                                                                             let _v18: *mut [i32; 4] = unsafe { _v17.add(0) };
// REWRITES-NEXT:                                                                                                             total = total + unsafe { (*_v18)[(_v13 as usize)] };
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                                                 k = k + 1;
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     j = j + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut t: table = table { rows: {{\[\[}}0; 3]; 4] };
// REWRITES-NEXT: let mut c: cube = cube { v: {{\[\[}}[0; 4]; 3]; 2] };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: fill(unsafe { &(*std::ptr::addr_of_mut!(t)) });
// REWRITES-NEXT: let _v1: i32 = 2;
// REWRITES-NEXT: fill_via_ptr(unsafe { &(*std::ptr::addr_of_mut!(t)) }, _v1);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v4: i32 = 4;
// REWRITES-NEXT:                     if !(i < _v4) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v6: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                     let _v9: *mut i8 = std::ptr::addr_of_mut!(t.rows[((i as i64) as usize)]) as *mut i8;
// REWRITES-NEXT:                                     let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: fill_cube(unsafe { &(*std::ptr::addr_of_mut!(c)) });
// REWRITES-NEXT: let _v13: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: i64 = 0;
// REWRITES-NEXT: let _v15: i64 = 0;
// REWRITES-NEXT: let _v16: i64 = 0;
// REWRITES-NEXT: let _v17: i32 = c.v[(_v16 as usize)][(_v15 as usize)][(_v14 as usize)];
// REWRITES-NEXT: let _v18: i64 = 3;
// REWRITES-NEXT: let _v19: i64 = 2;
// REWRITES-NEXT: let _v20: i64 = 1;
// REWRITES-NEXT: let _v21: i32 = c.v[(_v20 as usize)][(_v19 as usize)][(_v18 as usize)];
// REWRITES-NEXT: let _v22: i32 = sum_cube_via_ptr(unsafe { &(*std::ptr::addr_of_mut!(c)) });
// REWRITES-NEXT: let _v23: i32 = unsafe { printf(_v13 as *const i8, _v17, _v21, _v22) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
