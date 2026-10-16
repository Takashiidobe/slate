#include <stdlib.h>

int main(int argc, char **argv) {
  const int len = argc == 2 ? atoi(argv[1]) : 4;

  int count;
  int data[64];
  int M1[len][len];
  int M2[len][len];

  for (int i = 0; i < len; i++)
    for (int j = 0; j < len; j++)
      M1[i][j] = M2[i][j] = i * len + j;

  M2[1][0] = M2[0][1];

  /* This writes successively 0 and 1 into data[M2[0][1]].  */
  for (int i = 0; i < len - 1; i++)
    for (int j = 0; j < len; j++)
      if (M1[i + 1][j] > M1[i][j])
        data[M2[i][j]] = i;

  if (data[M2[0][1]] != 1)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn atoi(__nptr: *const i8) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut len: i32 = if argc == (2 as i32) {
// CHECK-NEXT:         unsafe { atoi((unsafe { *unsafe { argv.offset((1 as i32) as isize) } }) as *const i8) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         4 as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut data: __SlateAlign16<[i32; 64]> = __SlateAlign16([0 as i32; 64]);
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = (len as i64) as u64;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = (len as i64) as u64;
// CHECK-NEXT:     let mut M1: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity(([[V_0]] as usize) * ([[V_1]] as usize));
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla
// CHECK-NEXT:                 .as_mut_ptr()
// CHECK-NEXT:                 .write_bytes(0u8, ([[V_0]] as usize) * ([[V_1]] as usize));
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u64 = (len as i64) as u64;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = (len as i64) as u64;
// CHECK-NEXT:     let mut M2: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity(([[V_2]] as usize) * ([[V_3]] as usize));
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla
// CHECK-NEXT:                 .as_mut_ptr()
// CHECK-NEXT:                 .write_bytes(0u8, ([[V_2]] as usize) * ([[V_3]] as usize));
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:             let mut j: i32 = 0 as i32;
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < len {
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: i32 = i * len + j;
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         ((unsafe {
// CHECK-NEXT:                             (M2.as_mut_ptr() as *mut [i32; 0]).byte_offset(
// CHECK-NEXT:                                 (i as isize)
// CHECK-NEXT:                                     * ((([[V_3]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                             )
// CHECK-NEXT:                         }) as *mut i32)
// CHECK-NEXT:                             .offset(j as isize)
// CHECK-NEXT:                     } = [[V_4]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         ((unsafe {
// CHECK-NEXT:                             (M1.as_mut_ptr() as *mut [i32; 0]).byte_offset(
// CHECK-NEXT:                                 (i as isize)
// CHECK-NEXT:                                     * ((([[V_1]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                             )
// CHECK-NEXT:                         }) as *mut i32)
// CHECK-NEXT:                             .offset(j as isize)
// CHECK-NEXT:                     } = [[V_4]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:                 let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:                 j = [[V_6]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:             i = [[V_8]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             ((unsafe {
// CHECK-NEXT:                 (M2.as_mut_ptr() as *mut [i32; 0]).byte_offset(
// CHECK-NEXT:                     ((1 as i32) as isize)
// CHECK-NEXT:                         * ((([[V_3]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                 )
// CHECK-NEXT:             }) as *mut i32)
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 ((unsafe {
// CHECK-NEXT:                     (M2.as_mut_ptr() as *mut [i32; 0]).byte_offset(
// CHECK-NEXT:                         ((0 as i32) as isize)
// CHECK-NEXT:                             * ((([[V_3]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 }) as *mut i32)
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     // This writes successively 0 and 1 into data[M2[0][1]].
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < len - (1 as i32) {
// CHECK-NEXT:         let mut j: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while j < len {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     ((unsafe {
// CHECK-NEXT:                         (M1.as_mut_ptr() as *mut [i32; 0]).byte_offset(
// CHECK-NEXT:                             ((i + (1 as i32)) as isize)
// CHECK-NEXT:                                 * ((([[V_1]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                         )
// CHECK-NEXT:                     }) as *mut i32)
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) > unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     ((unsafe {
// CHECK-NEXT:                         (M1.as_mut_ptr() as *mut [i32; 0]).byte_offset(
// CHECK-NEXT:                             (i as isize)
// CHECK-NEXT:                                 * ((([[V_1]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                         )
// CHECK-NEXT:                     }) as *mut i32)
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             } {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (data.0.as_mut_ptr() as *mut i32).offset(
// CHECK-NEXT:                             (unsafe {
// CHECK-NEXT:                                 *unsafe {
// CHECK-NEXT:                                     ((unsafe {
// CHECK-NEXT:                                         (M2.as_mut_ptr() as *mut [i32; 0]).byte_offset(
// CHECK-NEXT:                                             (i as isize)
// CHECK-NEXT:                                                 * ((([[V_3]] as usize) * std::mem::size_of::<i32>())
// CHECK-NEXT:                                                     as isize),
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     }) as *mut i32)
// CHECK-NEXT:                                         .offset(j as isize)
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     } = i;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_9:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_10:__v[0-9]+]]: i32 = [[V_9]] + (1 as i32);
// CHECK-NEXT:             j = [[V_10]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: i32 = [[V_11]] + (1 as i32);
// CHECK-NEXT:         i = [[V_12]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             (data.0.as_mut_ptr() as *mut i32).offset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         ((unsafe {
// CHECK-NEXT:                             (M2.as_mut_ptr() as *mut [i32; 0]).byte_offset(
// CHECK-NEXT:                                 ((0 as i32) as isize)
// CHECK-NEXT:                                     * ((([[V_3]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                             )
// CHECK-NEXT:                         }) as *mut i32)
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as isize,
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
