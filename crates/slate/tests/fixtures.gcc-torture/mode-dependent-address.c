/* { dg-require-effective-target stdint_types } */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void f883b(int8_t *result, int16_t *__restrict arg1, uint32_t *__restrict arg2,
           uint64_t *__restrict arg3, uint8_t *__restrict arg4) {
  int idx;
  for (idx = 0; idx < 96; idx += 1) {
    result[idx] = (((((((((((-27 + 2 + 1) >> 1) || arg4[idx]) < arg1[idx])
                             ? (((-27 + 2 + 1) >> 1) || arg4[idx])
                             : arg1[idx]) >>
                        (arg2[idx] & 31)) ^
                       1) -
                      -32) >>
                     7) |
                    -5) &
                   arg3[idx]);
  }
}

int8_t   result[96];
int16_t  arg1[96];
uint32_t arg2[96];
uint64_t arg3[96];
uint8_t  arg4[96];

int main(void) {
  int i;
  int correct[] = {
      0x0,  0x1,  0x2,  0x3,  0x0,  0x1,  0x2,  0x3,  0x8,  0x9,  0xa,  0xb,
      0x8,  0x9,  0xa,  0xb,  0x10, 0x11, 0x12, 0x13, 0x10, 0x11, 0x12, 0x13,
      0x18, 0x19, 0x1a, 0x1b, 0x18, 0x19, 0x1a, 0x1b, 0x20, 0x21, 0x22, 0x23,
      0x20, 0x21, 0x22, 0x23, 0x28, 0x29, 0x2a, 0x2b, 0x28, 0x29, 0x2a, 0x2b,
      0x30, 0x31, 0x32, 0x33, 0x30, 0x31, 0x32, 0x33, 0x38, 0x39, 0x3a, 0x3b,
      0x38, 0x39, 0x3a, 0x3b, 0x40, 0x41, 0x42, 0x43, 0x40, 0x41, 0x42, 0x43,
      0x48, 0x49, 0x4a, 0x4b, 0x48, 0x49, 0x4a, 0x4b, 0x50, 0x51, 0x52, 0x53,
      0x50, 0x51, 0x52, 0x53, 0x58, 0x59, 0x5a, 0x5b, 0x58, 0x59, 0x5a, 0x5b};

  for (i = 0; i < 96; i++)
    arg3[i] = arg2[i] = arg1[i] = arg4[i] = i;

  f883b(result, arg1, arg2, arg3, arg4);

  for (i = 0; i < 96; i++)
    if (result[i] != correct[i])
      abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target stdint_types }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn f883b(
// CHECK-NEXT:     mut [[RESULT_0:result_[0-9]+]]: *mut i8,
// CHECK-NEXT:     mut [[ARG1_0:arg[0-9]+_[0-9]+]]: *mut i16,
// CHECK-NEXT:     mut [[ARG2_0:arg[0-9]+_[0-9]+]]: *mut u32,
// CHECK-NEXT:     mut [[ARG3_0:arg[0-9]+_[0-9]+]]: *mut u64,
// CHECK-NEXT:     mut [[ARG4_0:arg[0-9]+_[0-9]+]]: *mut u8,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut idx: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while idx < (96 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { [[RESULT_0]].offset(idx as isize) } =
// CHECK-NEXT:                 (((((((if ((-(27 as i32) + (2 as i32) + (1 as i32) >> (1 as i32) != (0 as i32)
// CHECK-NEXT:                     || (unsafe { *unsafe { [[ARG4_0]].offset(idx as isize) } }) != (0 as u8))
// CHECK-NEXT:                     as i32)
// CHECK-NEXT:                     < ((unsafe { *unsafe { [[ARG1_0]].offset(idx as isize) } }) as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     (-(27 as i32) + (2 as i32) + (1 as i32) >> (1 as i32) != (0 as i32)
// CHECK-NEXT:                         || (unsafe { *unsafe { [[ARG4_0]].offset(idx as isize) } }) != (0 as u8))
// CHECK-NEXT:                         as i32
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     (unsafe { *unsafe { [[ARG1_0]].offset(idx as isize) } }) as i32
// CHECK-NEXT:                 }) >> ((unsafe { *unsafe { [[ARG2_0]].offset(idx as isize) } })
// CHECK-NEXT:                     & ((31 as i32) as u32))
// CHECK-NEXT:                     ^ (1 as i32))
// CHECK-NEXT:                     - -(32 as i32)
// CHECK-NEXT:                     >> (7 as i32)
// CHECK-NEXT:                     | -(5 as i32)) as i64) as u64)
// CHECK-NEXT:                     & unsafe { *unsafe { [[ARG3_0]].offset(idx as isize) } }) as u8)
// CHECK-NEXT:                     as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = idx;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         idx = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut result: __SlateAlign16<[i8; 96]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut arg1: __SlateAlign16<[i16; 96]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut arg2: __SlateAlign16<[u32; 96]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut arg3: __SlateAlign16<[u64; 96]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut arg4: __SlateAlign16<[u8; 96]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut correct: __SlateAlign16<[i32; 96]> = __SlateAlign16([
// CHECK-NEXT:         0 as i32, 1 as i32, 2 as i32, 3 as i32, 0 as i32, 1 as i32, 2 as i32, 3 as i32, 8 as i32,
// CHECK-NEXT:         9 as i32, 10 as i32, 11 as i32, 8 as i32, 9 as i32, 10 as i32, 11 as i32, 16 as i32,
// CHECK-NEXT:         17 as i32, 18 as i32, 19 as i32, 16 as i32, 17 as i32, 18 as i32, 19 as i32, 24 as i32,
// CHECK-NEXT:         25 as i32, 26 as i32, 27 as i32, 24 as i32, 25 as i32, 26 as i32, 27 as i32, 32 as i32,
// CHECK-NEXT:         33 as i32, 34 as i32, 35 as i32, 32 as i32, 33 as i32, 34 as i32, 35 as i32, 40 as i32,
// CHECK-NEXT:         41 as i32, 42 as i32, 43 as i32, 40 as i32, 41 as i32, 42 as i32, 43 as i32, 48 as i32,
// CHECK-NEXT:         49 as i32, 50 as i32, 51 as i32, 48 as i32, 49 as i32, 50 as i32, 51 as i32, 56 as i32,
// CHECK-NEXT:         57 as i32, 58 as i32, 59 as i32, 56 as i32, 57 as i32, 58 as i32, 59 as i32, 64 as i32,
// CHECK-NEXT:         65 as i32, 66 as i32, 67 as i32, 64 as i32, 65 as i32, 66 as i32, 67 as i32, 72 as i32,
// CHECK-NEXT:         73 as i32, 74 as i32, 75 as i32, 72 as i32, 73 as i32, 74 as i32, 75 as i32, 80 as i32,
// CHECK-NEXT:         81 as i32, 82 as i32, 83 as i32, 80 as i32, 81 as i32, 82 as i32, 83 as i32, 88 as i32,
// CHECK-NEXT:         89 as i32, 90 as i32, 91 as i32, 88 as i32, 89 as i32, 90 as i32, 91 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (96 as i32) {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u8 = (i as i8) as u8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(arg4.0) as *mut u8 }.offset(i as isize) } =
// CHECK-NEXT:                 [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i16 = ([[V_2]] as u16) as i16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(arg1.0) as *mut i16 }.offset(i as isize) } =
// CHECK-NEXT:                 [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u32 = ([[V_3]] as i32) as u32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(arg2.0) as *mut u32 }.offset(i as isize) } =
// CHECK-NEXT:                 [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(arg3.0) as *mut u64 }.offset(i as isize) } =
// CHECK-NEXT:                 [[V_4]] as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     f883b(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(result.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(arg1.0) as *mut i16 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(arg2.0) as *mut u32 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(arg3.0) as *mut u64 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(arg4.0) as *mut u8 },
// CHECK-NEXT:     );
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (96 as i32) {
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(result.0) as *mut i8 }.offset(i as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != unsafe { *unsafe { (correct.0.as_mut_ptr() as *mut i32).offset(i as isize) } }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:         i = [[V_8]];
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
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
