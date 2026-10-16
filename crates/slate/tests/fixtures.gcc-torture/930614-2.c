void abort(void);
void exit(int);

int main(void) {
  int   i, j, k, l;
  float x[8][2][8][2];

  for (i = 0; i < 8; i++)
    for (j = i; j < 8; j++)
      for (k = 0; k < 2; k++)
        for (l = 0; l < 2; l++) {
          if ((i == j) && (k == l))
            x[i][k][j][l] = 0.8;
          else
            x[i][k][j][l] = 0.8;
          if (x[i][k][j][l] < 0.0)
            abort();
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut k: i32 = 0 as i32;
// CHECK-NEXT:     let mut l: i32 = 0 as i32;
// CHECK-NEXT:     let mut x: __SlateAlign16<{{\[\[}}{{\[\[}}f32; 2]; 8]; 2]; 8]> = __SlateAlign16(unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     });
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:         j = i;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < (8 as i32) {
// CHECK-NEXT:             k = 0 as i32;
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while k < (2 as i32) {
// CHECK-NEXT:                 l = 0 as i32;
// CHECK-NEXT:                 '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while l < (2 as i32) {
// CHECK-NEXT:                     if i == j && k == l {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             *unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             (*unsafe {
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     (*unsafe {
// CHECK-NEXT:                                                         (x.0.as_mut_ptr() as *mut {{\[\[}}[f32; 2]; 8]; 2])
// CHECK-NEXT:                                                             .offset(i as isize)
// CHECK-NEXT:                                                     })
// CHECK-NEXT:                                                         .as_mut_ptr() as *mut {{\[\[}}f32; 2]; 8]
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                                     .offset(k as isize)
// CHECK-NEXT:                                             })
// CHECK-NEXT:                                                 .as_mut_ptr() as *mut [f32; 2]
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                             .offset(j as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                         .as_mut_ptr() as *mut f32
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                     .offset(l as isize)
// CHECK-NEXT:                             } = 0.8f64 as f32;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             *unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             (*unsafe {
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     (*unsafe {
// CHECK-NEXT:                                                         (x.0.as_mut_ptr() as *mut {{\[\[}}[f32; 2]; 8]; 2])
// CHECK-NEXT:                                                             .offset(i as isize)
// CHECK-NEXT:                                                     })
// CHECK-NEXT:                                                         .as_mut_ptr() as *mut {{\[\[}}f32; 2]; 8]
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                                     .offset(k as isize)
// CHECK-NEXT:                                             })
// CHECK-NEXT:                                                 .as_mut_ptr() as *mut [f32; 2]
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                             .offset(j as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                         .as_mut_ptr() as *mut f32
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                     .offset(l as isize)
// CHECK-NEXT:                             } = 0.8f64 as f32;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         (*unsafe {
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 (*unsafe {
// CHECK-NEXT:                                                     (x.0.as_mut_ptr() as *mut {{\[\[}}[f32; 2]; 8]; 2])
// CHECK-NEXT:                                                         .offset(i as isize)
// CHECK-NEXT:                                                 })
// CHECK-NEXT:                                                     .as_mut_ptr() as *mut {{\[\[}}f32; 2]; 8]
// CHECK-NEXT:                                             }
// CHECK-NEXT:                                                 .offset(k as isize)
// CHECK-NEXT:                                         })
// CHECK-NEXT:                                             .as_mut_ptr() as *mut [f32; 2]
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                         .offset(j as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                     .as_mut_ptr() as *mut f32
// CHECK-NEXT:                             }
// CHECK-NEXT:                                 .offset(l as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as f64) < 0.0f64
// CHECK-NEXT:                     {
// CHECK-NEXT:                         unsafe { abort() };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_1:__v[0-9]+]]: i32 = l;
// CHECK-NEXT:                     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:                     l = [[V_2]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:                 k = [[V_4]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:             j = [[V_6]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:         i = [[V_8]];
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
