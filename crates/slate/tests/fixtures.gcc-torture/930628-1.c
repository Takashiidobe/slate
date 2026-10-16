void abort(void);
void exit(int);

void f(double x[2], double y[2]) {
  if (x == y)
    abort();
}

int main(void) {
  struct {
    int    f[3];
    double x[1][2];
  } tp[4][2];
  int   i, j, ki, kj, mi, mj;
  float bdm[4][2][4][2];

  for (i = 0; i < 4; i++)
    for (j = i; j < 4; j++)
      for (ki = 0; ki < 2; ki++)
        for (kj = 0; kj < 2; kj++)
          if ((j == i) && (ki == kj))
            bdm[i][ki][j][kj] = 1000.0;
          else {
            for (mi = 0; mi < 1; mi++)
              for (mj = 0; mj < 1; mj++)
                f(tp[i][ki].x[mi], tp[j][kj].x[mj]);
            bdm[i][ki][j][kj] = 1000.0;
          }
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     f: [i32; 3],
// CHECK-NEXT:     x: {{\[\[}}f64; 2]; 1],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn f(mut x: *mut f64, mut y: *mut f64) {
// CHECK-NEXT:     if x == y {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut tp: __SlateAlign16<{{\[\[}}[[SLATERECORD_0]]; 2]; 4]> = __SlateAlign16(unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut ki: i32 = 0 as i32;
// CHECK-NEXT:     let mut kj: i32 = 0 as i32;
// CHECK-NEXT:     let mut mi: i32 = 0 as i32;
// CHECK-NEXT:     let mut mj: i32 = 0 as i32;
// CHECK-NEXT:     let mut bdm: __SlateAlign16<{{\[\[}}{{\[\[}}f32; 2]; 4]; 2]; 4]> = __SlateAlign16(unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     });
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (4 as i32) {
// CHECK-NEXT:         j = i;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < (4 as i32) {
// CHECK-NEXT:             ki = 0 as i32;
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while ki < (2 as i32) {
// CHECK-NEXT:                 kj = 0 as i32;
// CHECK-NEXT:                 '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while kj < (2 as i32) {
// CHECK-NEXT:                     if j == i && ki == kj {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             *unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             (*unsafe {
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     (*unsafe {
// CHECK-NEXT:                                                         (bdm.0.as_mut_ptr() as *mut {{\[\[}}[f32; 2]; 4]; 2])
// CHECK-NEXT:                                                             .offset(i as isize)
// CHECK-NEXT:                                                     })
// CHECK-NEXT:                                                         .as_mut_ptr() as *mut {{\[\[}}f32; 2]; 4]
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                                     .offset(ki as isize)
// CHECK-NEXT:                                             })
// CHECK-NEXT:                                                 .as_mut_ptr() as *mut [f32; 2]
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                             .offset(j as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                         .as_mut_ptr() as *mut f32
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                     .offset(kj as isize)
// CHECK-NEXT:                             } = 1000.0f64 as f32;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         mi = 0 as i32;
// CHECK-NEXT:                         '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while mi < (1 as i32) {
// CHECK-NEXT:                             mj = 0 as i32;
// CHECK-NEXT:                             '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while mj < (1 as i32) {
// CHECK-NEXT:                                 f(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         (*unsafe {
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 (*unsafe {
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         (*unsafe {
// CHECK-NEXT:                                                             (tp.0.as_mut_ptr() as *mut [[[SLATERECORD_0]]; 2])
// CHECK-NEXT:                                                                 .offset(i as isize)
// CHECK-NEXT:                                                         })
// CHECK-NEXT:                                                             .as_mut_ptr() as *mut [[SLATERECORD_0]]
// CHECK-NEXT:                                                     }
// CHECK-NEXT:                                                         .offset(ki as isize)
// CHECK-NEXT:                                                 })
// CHECK-NEXT:                                                     .x
// CHECK-NEXT:                                                     .as_mut_ptr() as *mut [f64; 2]
// CHECK-NEXT:                                             }
// CHECK-NEXT:                                                 .offset(mi as isize)
// CHECK-NEXT:                                         })
// CHECK-NEXT:                                             .as_mut_ptr() as *mut f64
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         (*unsafe {
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 (*unsafe {
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         (*unsafe {
// CHECK-NEXT:                                                             (tp.0.as_mut_ptr() as *mut [[[SLATERECORD_0]]; 2])
// CHECK-NEXT:                                                                 .offset(j as isize)
// CHECK-NEXT:                                                         })
// CHECK-NEXT:                                                             .as_mut_ptr() as *mut [[SLATERECORD_0]]
// CHECK-NEXT:                                                     }
// CHECK-NEXT:                                                         .offset(kj as isize)
// CHECK-NEXT:                                                 })
// CHECK-NEXT:                                                     .x
// CHECK-NEXT:                                                     .as_mut_ptr() as *mut [f64; 2]
// CHECK-NEXT:                                             }
// CHECK-NEXT:                                                 .offset(mj as isize)
// CHECK-NEXT:                                         })
// CHECK-NEXT:                                             .as_mut_ptr() as *mut f64
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 );
// CHECK-NEXT:                                 let mut [[V_1:__v[0-9]+]]: i32 = mj;
// CHECK-NEXT:                                 let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:                                 mj = [[V_2]];
// CHECK-NEXT:                             }
// CHECK-NEXT:                             let mut [[V_3:__v[0-9]+]]: i32 = mi;
// CHECK-NEXT:                             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:                             mi = [[V_4]];
// CHECK-NEXT:                         }
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             *unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             (*unsafe {
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     (*unsafe {
// CHECK-NEXT:                                                         (bdm.0.as_mut_ptr() as *mut {{\[\[}}[f32; 2]; 4]; 2])
// CHECK-NEXT:                                                             .offset(i as isize)
// CHECK-NEXT:                                                     })
// CHECK-NEXT:                                                         .as_mut_ptr() as *mut {{\[\[}}f32; 2]; 4]
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                                     .offset(ki as isize)
// CHECK-NEXT:                                             })
// CHECK-NEXT:                                                 .as_mut_ptr() as *mut [f32; 2]
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                             .offset(j as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                         .as_mut_ptr() as *mut f32
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                     .offset(kj as isize)
// CHECK-NEXT:                             } = 1000.0f64 as f32;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_5:__v[0-9]+]]: i32 = kj;
// CHECK-NEXT:                     let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:                     kj = [[V_6]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_7:__v[0-9]+]]: i32 = ki;
// CHECK-NEXT:                 let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:                 ki = [[V_8]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_9:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_10:__v[0-9]+]]: i32 = [[V_9]] + (1 as i32);
// CHECK-NEXT:             j = [[V_10]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: i32 = [[V_11]] + (1 as i32);
// CHECK-NEXT:         i = [[V_12]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
