/* PR tree-optimization/70602 */
/* { dg-require-effective-target int32plus } */

struct __attribute__((packed)) S {
  int s : 1;
  int t : 20;
};

int a, b, c;

int main() {
  for (; a < 1; a++) {
    struct S e[] = {{0, 9}, {0, 9}, {0, 9}, {0, 0}, {0, 9}, {0, 9}, {0, 9},
                    {0, 0}, {0, 9}, {0, 9}, {0, 9}, {0, 0}, {0, 9}, {0, 9},
                    {0, 9}, {0, 0}, {0, 9}, {0, 9}, {0, 9}, {0, 0}, {0, 9}};
    b            = b || e[0].s;
    c            = e[0].t;
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/70602
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 3], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub s: i32,
// CHECK-NEXT:         #[bits(20)]
// CHECK-NEXT:         pub t: i32,
// CHECK-NEXT:         #[bits(3, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { a }) < (1 as i32) {
// CHECK-NEXT:         let mut e: __SlateAlign16<[S; 21]> = __SlateAlign16([
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_0:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_0]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_1:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_1]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_1]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_1]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_2:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_2]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_2]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_3:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_3]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_3]].__slate_bits_0.__set_t(0 as i32);
// CHECK-NEXT:                 [[T_3]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_4:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_4]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_4]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_5:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_5]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_5]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_5]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_6:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_6]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_6]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_6]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_7:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_7]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_7]].__slate_bits_0.__set_t(0 as i32);
// CHECK-NEXT:                 [[T_7]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_8:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_8]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_8]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_8]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_9:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_9]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_9]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_9]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_10:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_10]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_10]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_10]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_11:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_11]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_11]].__slate_bits_0.__set_t(0 as i32);
// CHECK-NEXT:                 [[T_11]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_12:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_12]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_12]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_12]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_13:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_13]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_13]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_13]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_14:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_14]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_14]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_14]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_15:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_15]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_15]].__slate_bits_0.__set_t(0 as i32);
// CHECK-NEXT:                 [[T_15]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_16:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_16]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_16]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_16]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_17:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_17]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_17]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_17]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_18:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_18]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_18]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_18]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_19:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_19]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_19]].__slate_bits_0.__set_t(0 as i32);
// CHECK-NEXT:                 [[T_19]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_20:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_20]].__slate_bits_0.__set_s(0 as i32);
// CHECK-NEXT:                 [[T_20]].__slate_bits_0.__set_t(9 as i32);
// CHECK-NEXT:                 [[T_20]]
// CHECK-NEXT:             },
// CHECK-NEXT:         ]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = ((unsafe { b }) != (0 as i32)
// CHECK-NEXT:                 || (unsafe {
// CHECK-NEXT:                     (*unsafe { (e.0.as_mut_ptr() as *mut S).offset((0 as i32) as isize) })
// CHECK-NEXT:                         .__slate_bits_0
// CHECK-NEXT:                         .__get_s()
// CHECK-NEXT:                 }) != (0 as i32)) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = unsafe {
// CHECK-NEXT:                 (*unsafe { (e.0.as_mut_ptr() as *mut S).offset((0 as i32) as isize) })
// CHECK-NEXT:                     .__slate_bits_0
// CHECK-NEXT:                     .__get_t()
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { a };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             a = [[V_1]];
// CHECK-NEXT:         }
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
