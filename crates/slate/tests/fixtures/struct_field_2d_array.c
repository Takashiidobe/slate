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

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct table {
// CHECK-NEXT:     rows: {{\[\[}}i8; 3]; 4],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct cube {
// CHECK-NEXT:     v: {{\[\[}}[i32; 4]; 3]; 2],
// CHECK-NEXT: }
// CHECK-NEXT: fn fill(t: &mut table) {
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { (*(t as *mut table)).rows.as_mut_ptr() as *mut [i8; 3] }
// CHECK-NEXT:                             .offset(i as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut i8
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset(0 as isize)
// CHECK-NEXT:             } = (97 + i) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { (*(t as *mut table)).rows.as_mut_ptr() as *mut [i8; 3] }
// CHECK-NEXT:                             .offset(i as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut i8
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset(1 as isize)
// CHECK-NEXT:             } = (48 + i) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { (*(t as *mut table)).rows.as_mut_ptr() as *mut [i8; 3] }
// CHECK-NEXT:                             .offset(i as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut i8
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset(2 as isize)
// CHECK-NEXT:             } = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn fill_via_ptr(t: &mut table, i: i32) {
// CHECK-NEXT:     let row: *mut [i8; 3] = unsafe {
// CHECK-NEXT:         (*(t as *mut table)).rows.as_mut_ptr() as *mut [i8; 3]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { unsafe { row.offset(i as isize) }.offset(0 as isize) })
// CHECK-NEXT:                     .as_mut_ptr() as *mut i8
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset(0 as isize)
// CHECK-NEXT:         } = 88;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn fill_cube(c: &mut cube) {
// CHECK-NEXT:     for i in 0 as i32..2 as i32 {
// CHECK-NEXT:         for j in 0 as i32..3 as i32 {
// CHECK-NEXT:             for k in 0 as i32..4 as i32 {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             (*(c as *mut cube)).v.as_mut_ptr() as *mut {{\[\[}}i32; 4]; 3]
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                             .offset(i as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                         .as_mut_ptr() as *mut [i32; 4]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                     .offset(j as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                                 .as_mut_ptr() as *mut i32
// CHECK-NEXT:                         }
// CHECK-NEXT:                             .offset(k as isize)
// CHECK-NEXT:                     } = i * 100 + j * 10 + k;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_cube_via_ptr(c: &mut cube) -> i32 {
// CHECK-NEXT:     let plane: *mut [i32; 4] = unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*(c as *mut cube)).v.as_mut_ptr() as *mut {{\[\[}}i32; 4]; 3] }
// CHECK-NEXT:                 .offset(1 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .as_mut_ptr() as *mut [i32; 4]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for j in 0 as i32..3 as i32 {
// CHECK-NEXT:         for k in 0 as i32..4 as i32 {
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:             total = [[V_0]]
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { plane.offset(j as isize) }.offset(0 as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                                 .as_mut_ptr() as *mut i32
// CHECK-NEXT:                         }
// CHECK-NEXT:                             .offset(k as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut t: table = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     fill(unsafe { &mut (*std::ptr::addr_of_mut!(t)) });
// CHECK-NEXT:     fill_via_ptr(unsafe { &mut (*std::ptr::addr_of_mut!(t)) }, 2 as i32);
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%s\n".as_ptr(), t.rows[i as usize].as_mut_ptr() as *mut i8) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut c: cube = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     fill_cube(unsafe { &mut (*std::ptr::addr_of_mut!(c)) });
// CHECK-NEXT:     let __slate_printf_arg2 = sum_cube_via_ptr(unsafe {
// CHECK-NEXT:         &mut (*std::ptr::addr_of_mut!(c))
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("{} {} {}", c.v[0] [0] [0], c.v[1] [2] [3], __slate_printf_arg2);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
