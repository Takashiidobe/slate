/* The purpose of this code is to test argument passing of a tuple of
   11 integers, with the break point between named and unnamed arguments
   at every possible position.	*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static int errors = 0;

static void verify(const char *tcase, int n[11]) {
  int i;
  for (i = 0; i <= 10; i++)
    if (n[i] != i) {
      printf(" %s: n[%d] = %d expected %d\n", tcase, i, n[i], i);
      errors++;
    }
}

#define STR(x) #x

#define p(i) int q##i,
#define P(i) n[i] = q##i;

#define p0 p(0)
#define p1 p(1)
#define p2 p(2)
#define p3 p(3)
#define p4 p(4)
#define p5 p(5)
#define p6 p(6)
#define p7 p(7)
#define p8 p(8)
#define p9 p(9)

#define P0 P(0)
#define P1 P(1)
#define P2 P(2)
#define P3 P(3)
#define P4 P(4)
#define P5 P(5)
#define P6 P(6)
#define P7 P(7)
#define P8 P(8)
#define P9 P(9)

#define TCASE(x, params, vecinit)                                              \
  static void varargs##x(params...) {                                          \
    va_list ap;                                                                \
    int     n[11];                                                             \
    int     i;                                                                 \
                                                                               \
    va_start(ap, q##x);                                                        \
    vecinit for (i = x + 1; i <= 10; i++) n[i] = va_arg(ap, int);              \
    va_end(ap);                                                                \
                                                                               \
    verify(STR(varargs##x), n);                                                \
  }

#define TEST(x) varargs##x(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)

TCASE(0, p0, P0)
TCASE(1, p0 p1, P0 P1)
TCASE(2, p0 p1 p2, P0 P1 P2)
TCASE(3, p0 p1 p2 p3, P0 P1 P2 P3)
TCASE(4, p0 p1 p2 p3 p4, P0 P1 P2 P3 P4)
TCASE(5, p0 p1 p2 p3 p4 p5, P0 P1 P2 P3 P4 P5)
TCASE(6, p0 p1 p2 p3 p4 p5 p6, P0 P1 P2 P3 P4 P5 P6)
TCASE(7, p0 p1 p2 p3 p4 p5 p6 p7, P0 P1 P2 P3 P4 P5 P6 P7)
TCASE(8, p0 p1 p2 p3 p4 p5 p6 p7 p8, P0 P1 P2 P3 P4 P5 P6 P7 P8)
TCASE(9, p0 p1 p2 p3 p4 p5 p6 p7 p8 p9, P0 P1 P2 P3 P4 P5 P6 P7 P8 P9)

int main(void) {
  TEST(0);
  TEST(1);
  TEST(2);
  TEST(3);
  TEST(4);
  TEST(5);
  TEST(6);
  TEST(7);
  TEST(8);
  TEST(9);

  if (errors)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! The purpose of this code is to test argument passing of a tuple of
// CHECK-NEXT: //! 11 integers, with the break point between named and unnamed arguments
// CHECK-NEXT: //! at every possible position.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut errors: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn verify(mut tcase: *const i8, mut n: *mut i32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         if (unsafe { *unsafe { n.offset(i as isize) } }) != i {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c" %s: n[%d] = %d expected %d\n".as_ptr() as *const i8,
// CHECK-NEXT:                     tcase,
// CHECK-NEXT:                     i,
// CHECK-NEXT:                     unsafe { *unsafe { n.offset(i as isize) } },
// CHECK-NEXT:                     i,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = unsafe { errors };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 errors = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn varargs0(mut q0: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n: __SlateAlign16<[i32; 11]> = __SlateAlign16([0 as i32; 11]);
// CHECK-NEXT:     let mut i: i32 = (0 as i32) + (1 as i32);
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = q0;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (n.0.as_mut_ptr() as *mut i32).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     verify(
// CHECK-NEXT:         c"varargs0".as_ptr() as *const i8,
// CHECK-NEXT:         n.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn varargs1(mut q0: i32, mut q1: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n: __SlateAlign16<[i32; 11]> = __SlateAlign16([0 as i32; 11]);
// CHECK-NEXT:     let mut i: i32 = (1 as i32) + (1 as i32);
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = q0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = q1;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (n.0.as_mut_ptr() as *mut i32).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         i = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     verify(
// CHECK-NEXT:         c"varargs1".as_ptr() as *const i8,
// CHECK-NEXT:         n.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn varargs2(mut q0: i32, mut q1: i32, mut q2: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n: __SlateAlign16<[i32; 11]> = __SlateAlign16([0 as i32; 11]);
// CHECK-NEXT:     let mut i: i32 = (2 as i32) + (1 as i32);
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = q0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = q1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } = q2;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (n.0.as_mut_ptr() as *mut i32).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:         i = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     verify(
// CHECK-NEXT:         c"varargs2".as_ptr() as *const i8,
// CHECK-NEXT:         n.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn varargs3(
// CHECK-NEXT:     mut q0: i32,
// CHECK-NEXT:     mut q1: i32,
// CHECK-NEXT:     mut q2: i32,
// CHECK-NEXT:     mut q3: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n: __SlateAlign16<[i32; 11]> = __SlateAlign16([0 as i32; 11]);
// CHECK-NEXT:     let mut i: i32 = (3 as i32) + (1 as i32);
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = q0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = q1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } = q2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } = q3;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (n.0.as_mut_ptr() as *mut i32).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] + (1 as i32);
// CHECK-NEXT:         i = [[V_11]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     verify(
// CHECK-NEXT:         c"varargs3".as_ptr() as *const i8,
// CHECK-NEXT:         n.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn varargs4(
// CHECK-NEXT:     mut q0: i32,
// CHECK-NEXT:     mut q1: i32,
// CHECK-NEXT:     mut q2: i32,
// CHECK-NEXT:     mut q3: i32,
// CHECK-NEXT:     mut q4: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n: __SlateAlign16<[i32; 11]> = __SlateAlign16([0 as i32; 11]);
// CHECK-NEXT:     let mut i: i32 = (4 as i32) + (1 as i32);
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = q0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = q1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } = q2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } = q3;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) } = q4;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (n.0.as_mut_ptr() as *mut i32).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: i32 = [[V_12]] + (1 as i32);
// CHECK-NEXT:         i = [[V_13]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     verify(
// CHECK-NEXT:         c"varargs4".as_ptr() as *const i8,
// CHECK-NEXT:         n.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn varargs5(
// CHECK-NEXT:     mut q0: i32,
// CHECK-NEXT:     mut q1: i32,
// CHECK-NEXT:     mut q2: i32,
// CHECK-NEXT:     mut q3: i32,
// CHECK-NEXT:     mut q4: i32,
// CHECK-NEXT:     mut q5: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n: __SlateAlign16<[i32; 11]> = __SlateAlign16([0 as i32; 11]);
// CHECK-NEXT:     let mut i: i32 = (5 as i32) + (1 as i32);
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = q0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = q1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } = q2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } = q3;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) } = q4;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((5 as i32) as isize) } = q5;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (n.0.as_mut_ptr() as *mut i32).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: i32 = [[V_14]] + (1 as i32);
// CHECK-NEXT:         i = [[V_15]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     verify(
// CHECK-NEXT:         c"varargs5".as_ptr() as *const i8,
// CHECK-NEXT:         n.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn varargs6(
// CHECK-NEXT:     mut q0: i32,
// CHECK-NEXT:     mut q1: i32,
// CHECK-NEXT:     mut q2: i32,
// CHECK-NEXT:     mut q3: i32,
// CHECK-NEXT:     mut q4: i32,
// CHECK-NEXT:     mut q5: i32,
// CHECK-NEXT:     mut q6: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n: __SlateAlign16<[i32; 11]> = __SlateAlign16([0 as i32; 11]);
// CHECK-NEXT:     let mut i: i32 = (6 as i32) + (1 as i32);
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = q0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = q1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } = q2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } = q3;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) } = q4;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((5 as i32) as isize) } = q5;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((6 as i32) as isize) } = q6;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (n.0.as_mut_ptr() as *mut i32).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_17:__v[0-9]+]]: i32 = [[V_16]] + (1 as i32);
// CHECK-NEXT:         i = [[V_17]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     verify(
// CHECK-NEXT:         c"varargs6".as_ptr() as *const i8,
// CHECK-NEXT:         n.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn varargs7(
// CHECK-NEXT:     mut q0: i32,
// CHECK-NEXT:     mut q1: i32,
// CHECK-NEXT:     mut q2: i32,
// CHECK-NEXT:     mut q3: i32,
// CHECK-NEXT:     mut q4: i32,
// CHECK-NEXT:     mut q5: i32,
// CHECK-NEXT:     mut q6: i32,
// CHECK-NEXT:     mut q7: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n: __SlateAlign16<[i32; 11]> = __SlateAlign16([0 as i32; 11]);
// CHECK-NEXT:     let mut i: i32 = (7 as i32) + (1 as i32);
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = q0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = q1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } = q2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } = q3;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) } = q4;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((5 as i32) as isize) } = q5;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((6 as i32) as isize) } = q6;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((7 as i32) as isize) } = q7;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (n.0.as_mut_ptr() as *mut i32).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_19:__v[0-9]+]]: i32 = [[V_18]] + (1 as i32);
// CHECK-NEXT:         i = [[V_19]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     verify(
// CHECK-NEXT:         c"varargs7".as_ptr() as *const i8,
// CHECK-NEXT:         n.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn varargs8(
// CHECK-NEXT:     mut q0: i32,
// CHECK-NEXT:     mut q1: i32,
// CHECK-NEXT:     mut q2: i32,
// CHECK-NEXT:     mut q3: i32,
// CHECK-NEXT:     mut q4: i32,
// CHECK-NEXT:     mut q5: i32,
// CHECK-NEXT:     mut q6: i32,
// CHECK-NEXT:     mut q7: i32,
// CHECK-NEXT:     mut q8: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n: __SlateAlign16<[i32; 11]> = __SlateAlign16([0 as i32; 11]);
// CHECK-NEXT:     let mut i: i32 = (8 as i32) + (1 as i32);
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = q0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = q1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } = q2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } = q3;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) } = q4;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((5 as i32) as isize) } = q5;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((6 as i32) as isize) } = q6;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((7 as i32) as isize) } = q7;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((8 as i32) as isize) } = q8;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (n.0.as_mut_ptr() as *mut i32).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_20:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_21:__v[0-9]+]]: i32 = [[V_20]] + (1 as i32);
// CHECK-NEXT:         i = [[V_21]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     verify(
// CHECK-NEXT:         c"varargs8".as_ptr() as *const i8,
// CHECK-NEXT:         n.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn varargs9(
// CHECK-NEXT:     mut q0: i32,
// CHECK-NEXT:     mut q1: i32,
// CHECK-NEXT:     mut q2: i32,
// CHECK-NEXT:     mut q3: i32,
// CHECK-NEXT:     mut q4: i32,
// CHECK-NEXT:     mut q5: i32,
// CHECK-NEXT:     mut q6: i32,
// CHECK-NEXT:     mut q7: i32,
// CHECK-NEXT:     mut q8: i32,
// CHECK-NEXT:     mut q9: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n: __SlateAlign16<[i32; 11]> = __SlateAlign16([0 as i32; 11]);
// CHECK-NEXT:     let mut i: i32 = (9 as i32) + (1 as i32);
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = q0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = q1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } = q2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } = q3;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) } = q4;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((5 as i32) as isize) } = q5;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((6 as i32) as isize) } = q6;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((7 as i32) as isize) } = q7;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((8 as i32) as isize) } = q8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (n.0.as_mut_ptr() as *mut i32).offset((9 as i32) as isize) } = q9;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (n.0.as_mut_ptr() as *mut i32).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_22:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_23:__v[0-9]+]]: i32 = [[V_22]] + (1 as i32);
// CHECK-NEXT:         i = [[V_23]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     verify(
// CHECK-NEXT:         c"varargs9".as_ptr() as *const i8,
// CHECK-NEXT:         n.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         varargs0(
// CHECK-NEXT:             0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32,
// CHECK-NEXT:             8 as i32, 9 as i32, 10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         varargs1(
// CHECK-NEXT:             0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32,
// CHECK-NEXT:             8 as i32, 9 as i32, 10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         varargs2(
// CHECK-NEXT:             0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32,
// CHECK-NEXT:             8 as i32, 9 as i32, 10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         varargs3(
// CHECK-NEXT:             0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32,
// CHECK-NEXT:             8 as i32, 9 as i32, 10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         varargs4(
// CHECK-NEXT:             0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32,
// CHECK-NEXT:             8 as i32, 9 as i32, 10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         varargs5(
// CHECK-NEXT:             0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32,
// CHECK-NEXT:             8 as i32, 9 as i32, 10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         varargs6(
// CHECK-NEXT:             0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32,
// CHECK-NEXT:             8 as i32, 9 as i32, 10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         varargs7(
// CHECK-NEXT:             0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32,
// CHECK-NEXT:             8 as i32, 9 as i32, 10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         varargs8(
// CHECK-NEXT:             0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32,
// CHECK-NEXT:             8 as i32, 9 as i32, 10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         varargs9(
// CHECK-NEXT:             0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32,
// CHECK-NEXT:             8 as i32, 9 as i32, 10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe { errors }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
