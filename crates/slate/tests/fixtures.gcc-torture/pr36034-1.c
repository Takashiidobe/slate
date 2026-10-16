double x[5][10] = {{10, 11, 12, 13, 14, 15, -1, -1, -1, -1},
                   {21, 22, 23, 24, 25, 26, -1, -1, -1, -1},
                   {32, 33, 34, 35, 36, 37, -1, -1, -1, -1},
                   {43, 44, 45, 46, 47, 48, -1, -1, -1, -1},
                   {54, 55, 56, 57, 58, 59, -1, -1, -1, -1}};
double tmp[5][6];

void __attribute__((noinline)) test(void) {
  int i, j;
  for (i = 0; i < 5; ++i) {
    tmp[i][0] = x[i][0];
    tmp[i][1] = x[i][1];
    tmp[i][2] = x[i][2];
    tmp[i][3] = x[i][3];
    tmp[i][4] = x[i][4];
    tmp[i][5] = x[i][5];
  }
}
extern void abort(void);
int         main() {
  int i, j;
  test();
  for (i = 0; i < 5; ++i)
    for (j = 0; j < 6; ++j)
      if (tmp[i][j] == -1)
        abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut x: __SlateAlign16<{{\[\[}}f64; 10]; 5]> = __SlateAlign16([
// CHECK-NEXT:     [
// CHECK-NEXT:         (10 as i32) as f64,
// CHECK-NEXT:         (11 as i32) as f64,
// CHECK-NEXT:         (12 as i32) as f64,
// CHECK-NEXT:         (13 as i32) as f64,
// CHECK-NEXT:         (14 as i32) as f64,
// CHECK-NEXT:         (15 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:     ],
// CHECK-NEXT:     [
// CHECK-NEXT:         (21 as i32) as f64,
// CHECK-NEXT:         (22 as i32) as f64,
// CHECK-NEXT:         (23 as i32) as f64,
// CHECK-NEXT:         (24 as i32) as f64,
// CHECK-NEXT:         (25 as i32) as f64,
// CHECK-NEXT:         (26 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:     ],
// CHECK-NEXT:     [
// CHECK-NEXT:         (32 as i32) as f64,
// CHECK-NEXT:         (33 as i32) as f64,
// CHECK-NEXT:         (34 as i32) as f64,
// CHECK-NEXT:         (35 as i32) as f64,
// CHECK-NEXT:         (36 as i32) as f64,
// CHECK-NEXT:         (37 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:     ],
// CHECK-NEXT:     [
// CHECK-NEXT:         (43 as i32) as f64,
// CHECK-NEXT:         (44 as i32) as f64,
// CHECK-NEXT:         (45 as i32) as f64,
// CHECK-NEXT:         (46 as i32) as f64,
// CHECK-NEXT:         (47 as i32) as f64,
// CHECK-NEXT:         (48 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:     ],
// CHECK-NEXT:     [
// CHECK-NEXT:         (54 as i32) as f64,
// CHECK-NEXT:         (55 as i32) as f64,
// CHECK-NEXT:         (56 as i32) as f64,
// CHECK-NEXT:         (57 as i32) as f64,
// CHECK-NEXT:         (58 as i32) as f64,
// CHECK-NEXT:         (59 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:         -(1 as i32) as f64,
// CHECK-NEXT:     ],
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut tmp: __SlateAlign16<{{\[\[}}f64; 6]; 5]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn test() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (5 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut [f64; 6] }
// CHECK-NEXT:                             .offset(i as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut f64
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:             } = unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(x.0) as *mut [f64; 10] }
// CHECK-NEXT:                                 .offset(i as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                             .as_mut_ptr() as *mut f64
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut [f64; 6] }
// CHECK-NEXT:                             .offset(i as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut f64
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             } = unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(x.0) as *mut [f64; 10] }
// CHECK-NEXT:                                 .offset(i as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                             .as_mut_ptr() as *mut f64
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut [f64; 6] }
// CHECK-NEXT:                             .offset(i as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut f64
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             } = unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(x.0) as *mut [f64; 10] }
// CHECK-NEXT:                                 .offset(i as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                             .as_mut_ptr() as *mut f64
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut [f64; 6] }
// CHECK-NEXT:                             .offset(i as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut f64
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             } = unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(x.0) as *mut [f64; 10] }
// CHECK-NEXT:                                 .offset(i as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                             .as_mut_ptr() as *mut f64
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut [f64; 6] }
// CHECK-NEXT:                             .offset(i as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut f64
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             } = unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(x.0) as *mut [f64; 10] }
// CHECK-NEXT:                                 .offset(i as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                             .as_mut_ptr() as *mut f64
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut [f64; 6] }
// CHECK-NEXT:                             .offset(i as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .as_mut_ptr() as *mut f64
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             } = unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(x.0) as *mut [f64; 10] }
// CHECK-NEXT:                                 .offset(i as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                             .as_mut_ptr() as *mut f64
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     test();
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (5 as i32) {
// CHECK-NEXT:         j = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while j < (6 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut [f64; 6] }
// CHECK-NEXT:                                 .offset(i as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                             .as_mut_ptr() as *mut f64
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) == (-(1 as i32) as f64)
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             j = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
