/* PR tree-optimization/86622 - incorrect strlen of array of array plus
   variable offset
   Exercise strlen() with a multi-dimensional array of strings with
   offsets.  */

extern int           printf(const char *, ...);
extern __SIZE_TYPE__ strlen(const char *);

typedef char  A28[28];
typedef A28   A3_28[3];
typedef A3_28 A2_3_28[2];

static const A2_3_28 a = {
    /* [0][0]    [0][1]         [0][2] */
    {"1\00012", "123\0001234", "12345\000123456"},
    /* [1][0]    [1][1]         [1][2] */
    {"1234567\00012345678", "123456789\0001234567890",
     "12345678901\000123456789012"}};

volatile int v0 = 0;
volatile int v1 = 1;
volatile int v2 = 2;
volatile int v3 = 3;
volatile int v4 = 4;
volatile int v5 = 5;
volatile int v6 = 6;
volatile int v7 = 7;

#define A(expr, N)                                                             \
  ((strlen(expr) == N) ? (void)0                                               \
                       : (printf("line %i: strlen (%s = \"%s\") != %i\n",      \
                                 __LINE__, #expr, expr, N),                    \
                          __builtin_abort()))

/* Verify that strlen() involving pointer to array arguments computes
   the correct result.  */

void test_array_ptr(void) {
  /* Compute the length of the string at the refeenced array.  */
  A(*(&a[0][0] + 0), 1);
  A(*(&a[0][0] + 1), 3);
  A(*(&a[0][0] + 2), 5);

  A(*(&a[0][1] - 1), 1);
  A(*(&a[0][1] + 0), 3);
  A(*(&a[0][1] + 1), 5);

  A(*(&a[0][2] - 2), 1);
  A(*(&a[0][2] - 1), 3);
  A(*(&a[0][2] + 0), 5);

  A(*(&a[1][0] + 0), 7);
  A(*(&a[1][0] + 1), 9);
  A(*(&a[1][0] + 2), 11);

  A(*(&a[1][1] - 1), 7);
  A(*(&a[1][1] + 0), 9);
  A(*(&a[1][1] + 1), 11);

  A(*(&a[1][2] - 2), 7);
  A(*(&a[1][2] - 1), 9);
  A(*(&a[1][2] - 0), 11);

  /* Compute the length of the string past the first nul.  */
  A(*(&a[0][0] + 0) + 2, 2);
  A(*(&a[0][0] + 1) + 4, 4);
  A(*(&a[0][0] + 2) + 6, 6);

  /* Compute the length of the string past the second nul.  */
  A(*(&a[0][0] + 0) + 5, 0);
  A(*(&a[0][0] + 1) + 10, 0);
  A(*(&a[0][0] + 2) + 14, 0);

  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;
  int i3 = i2 + 1;
  int i4 = i3 + 1;
  int i5 = i4 + 1;

  A(*(&a[0][0] + i0), 1);
  A(*(&a[0][0] + i1), 3);
  A(*(&a[0][0] + i2), 5);

  A(*(&a[0][1] - i1), 1);
  A(*(&a[0][1] + i0), 3);
  A(*(&a[0][1] + i1), 5);

  A(*(&a[0][2] - i2), 1);
  A(*(&a[0][2] - i1), 3);
  A(*(&a[0][2] + i0), 5);

  A(*(&a[1][0] + i0), 7);
  A(*(&a[1][0] + i1), 9);
  A(*(&a[1][0] + i2), 11);

  A(*(&a[1][1] - i1), 7);
  A(*(&a[1][1] + i0), 9);
  A(*(&a[1][1] + i1), 11);

  A(*(&a[1][2] - i2), 7);
  A(*(&a[1][2] - i1), 9);
  A(*(&a[1][2] - i0), 11);

  A(*(&a[i0][i0] + i0), 1);
  A(*(&a[i0][i0] + i1), 3);
  A(*(&a[i0][i0] + i2), 5);

  A(*(&a[i0][i1] - i1), 1);
  A(*(&a[i0][i1] + i0), 3);
  A(*(&a[i0][i1] + i1), 5);

  A(*(&a[i0][i2] - i2), 1);
  A(*(&a[i0][i2] - i1), 3);
  A(*(&a[i0][i2] + i0), 5);

  A(*(&a[i1][i0] + i0), 7);
  A(*(&a[i1][i0] + i1), 9);
  A(*(&a[i1][i0] + i2), 11);

  A(*(&a[i1][i1] - i1), 7);
  A(*(&a[i1][i1] + i0), 9);
  A(*(&a[i1][i1] + i1), 11);

  A(*(&a[i1][i2] - i2), 7);
  A(*(&a[i1][i2] - i1), 9);
  A(*(&a[i1][i2] - i0), 11);

  A(*(&a[i0][i0] + v0), 1);
  A(*(&a[i0][i0] + v1), 3);
  A(*(&a[i0][i0] + v2), 5);

  A(*(&a[i0][i1] - v1), 1);
  A(*(&a[i0][i1] + v0), 3);
  A(*(&a[i0][i1] + v1), 5);

  A(*(&a[i0][i2] - v2), 1);
  A(*(&a[i0][i2] - v1), 3);
  A(*(&a[i0][i2] + v0), 5);

  A(*(&a[i1][i0] + v0), 7);
  A(*(&a[i1][i0] + v1), 9);
  A(*(&a[i1][i0] + v2), 11);

  A(*(&a[i1][i1] - v1), 7);
  A(*(&a[i1][i1] + v0), 9);
  A(*(&a[i1][i1] + v1), 11);

  A(*(&a[i1][i2] - v2), 7);
  A(*(&a[i1][i2] - v1), 9);
  A(*(&a[i1][i2] - v0), 11);

  A(*(&a[i0][i0] + v0) + i1, 0);
  A(*(&a[i0][i0] + v1) + i2, 1);
  A(*(&a[i0][i0] + v2) + i3, 2);

  A(*(&a[i0][i1] - v1) + v1, 0);
  A(*(&a[i0][i1] + v0) + v3, 0);
  A(*(&a[i0][i1] + v1) + v5, 0);

  A(*(&a[i0][v1] - i1) + i1, 0);
  A(*(&a[i0][v1] + i0) + i3, 0);
  A(*(&a[i0][v1] + i1) + i5, 0);
}

static const A3_28 *const pa0 = &a[0];
static const A3_28 *const pa1 = &a[1];

static const A3_28 *const paa[] = {&a[0], &a[1]};

/* Verify that strlen() involving pointers and arrays of pointers
   to array arguments computes the correct result.  */

void test_ptr_array(void) {
  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;
  int i3 = i2 + 1;

  A(*((*pa0) + i0), 1);
  A(*((*pa0) + i1), 3);
  A(*((*pa0) + i2), 5);

  A(*(pa0[0] + i0), 1);
  A(*(pa0[0] + i1), 3);
  A(*(pa0[0] + i2), 5);

  A((*pa0)[i0] + i1, 0);
  A((*pa0)[i1] + i2, 1);
  A((*pa0)[i2] + i3, 2);

  A(*((*pa1) + i0), 7);
  A(*((*pa1) + i1), 9);
  A(*((*pa1) + i2), 11);

  A(*(pa1[0] + i0), 7);
  A(*(pa1[0] + i1), 9);
  A(*(pa1[0] + i2), 11);

  A((*pa1)[i0] + i1, 6);
  A((*pa1)[i1] + i2, 7);
  A((*pa1)[i2] + i3, 8);

  A(*(*(paa[0]) + i0), 1);
  A(*(*(paa[0]) + i1), 3);
  A(*(*(paa[0]) + i2), 5);

  A(*(*(paa[1]) + i0), 7);
  A(*(*(paa[1]) + i1), 9);
  A(*(*(paa[1]) + i2), 11);

  A(*(*(paa[1]) - i1), 5);
  A(*(*(paa[1]) - i2), 3);
  A(*(*(paa[1]) - i3), 1);

  A(*(*(paa[0]) + i0) + i1, 0);
  A(*(*(paa[0]) + i1) + i2, 1);
  A(*(*(paa[0]) + i2) + i3, 2);
}

int main(void) {
  test_array_ptr();

  test_ptr_array();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/86622 - incorrect strlen of array of array plus
// CHECK-NEXT: //! variable offset
// CHECK-NEXT: //! Exercise strlen() with a multi-dimensional array of strings with
// CHECK-NEXT: //! offsets.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn strlen([[V_1:__v[0-9]+]]: *const i8) -> u64;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: /// [0][0]    [0][1]         [0][2]
// CHECK-NEXT: /// [1][0]    [1][1]         [1][2]
// CHECK-NEXT: static mut a: __SlateAlign16<{{\[\[}}[i8; 28]; 3]; 2]> = __SlateAlign16([
// CHECK-NEXT:     [
// CHECK-NEXT:         [
// CHECK-NEXT:             49 as i8, 0 as i8, 49 as i8, 50 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:             0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:             0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:             0 as i8,
// CHECK-NEXT:         ],
// CHECK-NEXT:         [
// CHECK-NEXT:             49 as i8, 50 as i8, 51 as i8, 0 as i8, 49 as i8, 50 as i8, 51 as i8, 52 as i8, 0 as i8,
// CHECK-NEXT:             0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:             0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:             0 as i8,
// CHECK-NEXT:         ],
// CHECK-NEXT:         [
// CHECK-NEXT:             49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 0 as i8, 49 as i8, 50 as i8,
// CHECK-NEXT:             51 as i8, 52 as i8, 53 as i8, 54 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:             0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:             0 as i8, 0 as i8,
// CHECK-NEXT:         ],
// CHECK-NEXT:     ],
// CHECK-NEXT:     [
// CHECK-NEXT:         [
// CHECK-NEXT:             49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 54 as i8, 55 as i8, 0 as i8,
// CHECK-NEXT:             49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 54 as i8, 55 as i8, 56 as i8,
// CHECK-NEXT:             0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:             0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         ],
// CHECK-NEXT:         [
// CHECK-NEXT:             49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 54 as i8, 55 as i8, 56 as i8,
// CHECK-NEXT:             57 as i8, 0 as i8, 49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 54 as i8,
// CHECK-NEXT:             55 as i8, 56 as i8, 57 as i8, 48 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:             0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         ],
// CHECK-NEXT:         [
// CHECK-NEXT:             49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 54 as i8, 55 as i8, 56 as i8,
// CHECK-NEXT:             57 as i8, 48 as i8, 49 as i8, 0 as i8, 49 as i8, 50 as i8, 51 as i8, 52 as i8,
// CHECK-NEXT:             53 as i8, 54 as i8, 55 as i8, 56 as i8, 57 as i8, 48 as i8, 49 as i8, 50 as i8,
// CHECK-NEXT:             0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         ],
// CHECK-NEXT:     ],
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut v0: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut v1: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut v2: i32 = 2 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut v3: i32 = 3 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut v4: i32 = 4 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut v5: i32 = 5 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut v6: i32 = 6 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut v7: i32 = 7 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: // Verify that strlen() involving pointer to array arguments computes
// CHECK-NEXT: // the correct result.
// CHECK-NEXT: fn test_array_ptr() {
// CHECK-NEXT:     // Compute the length of the string at the refeenced array.
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 40 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + 0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 41 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + 1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 42 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + 2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((1 as i32) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 44 as i32,
// CHECK-NEXT:                 c"*(&a[0][1] - 1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-((1 as i32) as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 45 as i32,
// CHECK-NEXT:                 c"*(&a[0][1] + 0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 46 as i32,
// CHECK-NEXT:                 c"*(&a[0][1] + 1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((2 as i32) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 48 as i32,
// CHECK-NEXT:                 c"*(&a[0][2] - 2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-((2 as i32) as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((1 as i32) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 49 as i32,
// CHECK-NEXT:                 c"*(&a[0][2] - 1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-((1 as i32) as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 50 as i32,
// CHECK-NEXT:                 c"*(&a[0][2] + 0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 52 as i32,
// CHECK-NEXT:                 c"*(&a[1][0] + 0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 53 as i32,
// CHECK-NEXT:                 c"*(&a[1][0] + 1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 54 as i32,
// CHECK-NEXT:                 c"*(&a[1][0] + 2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((1 as i32) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 56 as i32,
// CHECK-NEXT:                 c"*(&a[1][1] - 1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-((1 as i32) as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 57 as i32,
// CHECK-NEXT:                 c"*(&a[1][1] + 0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 58 as i32,
// CHECK-NEXT:                 c"*(&a[1][1] + 1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((2 as i32) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 60 as i32,
// CHECK-NEXT:                 c"*(&a[1][2] - 2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-((2 as i32) as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((1 as i32) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 61 as i32,
// CHECK-NEXT:                 c"*(&a[1][2] - 1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-((1 as i32) as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((0 as i32) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 62 as i32,
// CHECK-NEXT:                 c"*(&a[1][2] - 0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-((0 as i32) as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // Compute the length of the string past the first nul.
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((2 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 65 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + 0) + 2".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset((0 as i32) as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 2 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((4 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + 1) + 4".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset((0 as i32) as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 4 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((6 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((6 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 67 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + 2) + 6".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset((0 as i32) as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 6 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // Compute the length of the string past the second nul.
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((5 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 70 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + 0) + 5".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset((0 as i32) as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((10 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 71 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + 1) + 10".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset((0 as i32) as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((14 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 72 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + 2) + 14".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset((0 as i32) as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i0: i32 = 0 as i32;
// CHECK-NEXT:     let mut i1: i32 = i0 + (1 as i32);
// CHECK-NEXT:     let mut i2: i32 = i1 + (1 as i32);
// CHECK-NEXT:     let mut i3: i32 = i2 + (1 as i32);
// CHECK-NEXT:     let mut i4: i32 = i3 + (1 as i32);
// CHECK-NEXT:     let mut i5: i32 = i4 + (1 as i32);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 81 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 82 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i2 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 83 as i32,
// CHECK-NEXT:                 c"*(&a[0][0] + i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i2 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i1 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 85 as i32,
// CHECK-NEXT:                 c"*(&a[0][1] - i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i1 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 86 as i32,
// CHECK-NEXT:                 c"*(&a[0][1] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 87 as i32,
// CHECK-NEXT:                 c"*(&a[0][1] + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i2 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 89 as i32,
// CHECK-NEXT:                 c"*(&a[0][2] - i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i2 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i1 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 90 as i32,
// CHECK-NEXT:                 c"*(&a[0][2] - i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i1 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 91 as i32,
// CHECK-NEXT:                 c"*(&a[0][2] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 93 as i32,
// CHECK-NEXT:                 c"*(&a[1][0] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 94 as i32,
// CHECK-NEXT:                 c"*(&a[1][0] + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i2 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 95 as i32,
// CHECK-NEXT:                 c"*(&a[1][0] + i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i2 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i1 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 97 as i32,
// CHECK-NEXT:                 c"*(&a[1][1] - i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i1 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 98 as i32,
// CHECK-NEXT:                 c"*(&a[1][1] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 99 as i32,
// CHECK-NEXT:                 c"*(&a[1][1] + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i2 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 101 as i32,
// CHECK-NEXT:                 c"*(&a[1][2] - i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i2 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i1 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 102 as i32,
// CHECK-NEXT:                 c"*(&a[1][2] - i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i1 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i0 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 103 as i32,
// CHECK-NEXT:                 c"*(&a[1][2] - i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset((1 as i32) as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i0 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 105 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i0] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 106 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i0] + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i2 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 107 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i0] + i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i2 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i1 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 109 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i1] - i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i1 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 110 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i1] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 111 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i1] + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i2 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 113 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i2] - i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i2 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i1 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 114 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i2] - i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i1 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 115 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i2] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 117 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i0] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 118 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i0] + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i2 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 119 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i0] + i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i2 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i1 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 121 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i1] - i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i1 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 122 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i1] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 123 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i1] + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i2 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 125 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i2] - i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i2 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i1 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 126 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i2] - i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i1 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i0 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 127 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i2] - i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i0 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 129 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i0] + v0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 130 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i0] + v1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v2)) }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 131 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i0] + v2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v2)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 133 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i1] - v1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             -((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) })
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 134 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i1] + v0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 135 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i1] + v1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v2)) }) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 137 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i2] - v2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             -((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v2)) })
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 138 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i2] - v1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             -((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) })
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 139 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i2] + v0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i0 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 141 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i0] + v0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 142 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i0] + v1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v2)) }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 143 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i0] + v2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v2)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 145 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i1] - v1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             -((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) })
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 146 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i1] + v0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 147 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i1] + v1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v2)) }) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 149 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i2] - v2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             -((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v2)) })
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 150 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i2] - v1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             -((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) })
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 151 as i32,
// CHECK-NEXT:                 c"*(&a[i1][i2] - v0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                         .offset(i1 as isize)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             -((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) })
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                         )
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset(i0 as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i1 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 153 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i0] + v0) + i1".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset(i0 as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(
// CHECK-NEXT:                                 (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) })
// CHECK-NEXT:                                     as isize,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset(i0 as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i2 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 154 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i0] + v1) + i2".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset(i0 as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(
// CHECK-NEXT:                                 (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) })
// CHECK-NEXT:                                     as isize,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset(i0 as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v2)) }) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i3 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 155 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i0] + v2) + i3".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset(i0 as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(
// CHECK-NEXT:                                 (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v2)) })
// CHECK-NEXT:                                     as isize,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i3 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 2 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset(i0 as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(
// CHECK-NEXT:                         -((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 157 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i1] - v1) + v1".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset(i0 as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(
// CHECK-NEXT:                                 -((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) })
// CHECK-NEXT:                                     as isize),
// CHECK-NEXT:                             )
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset(i0 as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) }) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v3)) }) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 158 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i1] + v0) + v3".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset(i0 as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(
// CHECK-NEXT:                                 (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v0)) })
// CHECK-NEXT:                                     as isize,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v3)) }) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset(i0 as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v5)) }) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 159 as i32,
// CHECK-NEXT:                 c"*(&a[i0][i1] + v1) + v5".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset(i0 as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(
// CHECK-NEXT:                                 (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) })
// CHECK-NEXT:                                     as isize,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v5)) }) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset(i0 as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(-(i1 as isize))
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i1 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 161 as i32,
// CHECK-NEXT:                 c"*(&a[i0][v1] - i1) + i1".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset(i0 as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset(
// CHECK-NEXT:                                     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) })
// CHECK-NEXT:                                         as isize,
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(-(i1 as isize))
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset(i0 as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i3 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 162 as i32,
// CHECK-NEXT:                 c"*(&a[i0][v1] + i0) + i3".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset(i0 as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset(
// CHECK-NEXT:                                     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) })
// CHECK-NEXT:                                         as isize,
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i3 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset(i0 as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i5 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 163 as i32,
// CHECK-NEXT:                 c"*(&a[i0][v1] + i1) + i5".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     (*unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                             .offset(i0 as isize)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 .offset(
// CHECK-NEXT:                                     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v1)) })
// CHECK-NEXT:                                         as isize,
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i5 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut pa0: *const {{\[\[}}i8; 28]; 3] = unsafe {
// CHECK-NEXT:     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }.offset((0 as i32) as isize)
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut pa1: *const {{\[\[}}i8; 28]; 3] = unsafe {
// CHECK-NEXT:     unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }.offset((1 as i32) as isize)
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut paa: __SlateAlign16<[*const {{\[\[}}i8; 28]; 3]; 2]> = __SlateAlign16([
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }.offset((0 as i32) as isize)
// CHECK-NEXT:     },
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of!(a.0) as *const {{\[\[}}i8; 28]; 3] }.offset((1 as i32) as isize)
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: // Verify that strlen() involving pointers and arrays of pointers
// CHECK-NEXT: // to array arguments computes the correct result.
// CHECK-NEXT: fn test_ptr_array() {
// CHECK-NEXT:     let mut i0: i32 = 0 as i32;
// CHECK-NEXT:     let mut i1: i32 = i0 + (1 as i32);
// CHECK-NEXT:     let mut i2: i32 = i1 + (1 as i32);
// CHECK-NEXT:     let mut i3: i32 = i2 + (1 as i32);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }.offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 180 as i32,
// CHECK-NEXT:                 c"*((*pa0) + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }.offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }.offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 181 as i32,
// CHECK-NEXT:                 c"*((*pa0) + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }.offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }.offset(i2 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 182 as i32,
// CHECK-NEXT:                 c"*((*pa0) + i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }.offset(i2 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe { unsafe { pa0 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                         as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 184 as i32,
// CHECK-NEXT:                 c"*(pa0[0] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe { unsafe { pa0 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                                 as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe { unsafe { pa0 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                         as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 185 as i32,
// CHECK-NEXT:                 c"*(pa0[0] + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe { unsafe { pa0 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                                 as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe { unsafe { pa0 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                         as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i2 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 186 as i32,
// CHECK-NEXT:                 c"*(pa0[0] + i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe { unsafe { pa0 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                                 as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i2 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }.offset(i0 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i1 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 188 as i32,
// CHECK-NEXT:                 c"(*pa0)[i0] + i1".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }.offset(i1 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i2 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 189 as i32,
// CHECK-NEXT:                 c"(*pa0)[i1] + i2".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }.offset(i2 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i3 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 190 as i32,
// CHECK-NEXT:                 c"(*pa0)[i2] + i3".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { (*unsafe { pa0 }).as_ptr() as *const [i8; 28] }
// CHECK-NEXT:                                 .offset(i2 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i3 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 2 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }.offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 192 as i32,
// CHECK-NEXT:                 c"*((*pa1) + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }.offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }.offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 193 as i32,
// CHECK-NEXT:                 c"*((*pa1) + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }.offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }.offset(i2 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 194 as i32,
// CHECK-NEXT:                 c"*((*pa1) + i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }.offset(i2 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe { unsafe { pa1 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                         as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 196 as i32,
// CHECK-NEXT:                 c"*(pa1[0] + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe { unsafe { pa1 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                                 as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe { unsafe { pa1 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                         as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 197 as i32,
// CHECK-NEXT:                 c"*(pa1[0] + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe { unsafe { pa1 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                                 as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe { unsafe { pa1 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                         as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i2 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 198 as i32,
// CHECK-NEXT:                 c"*(pa1[0] + i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe { unsafe { pa1 }.offset((0 as i32) as isize) }).as_ptr()
// CHECK-NEXT:                                 as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i2 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }.offset(i0 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i1 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((6 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 200 as i32,
// CHECK-NEXT:                 c"(*pa1)[i0] + i1".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }
// CHECK-NEXT:                                 .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 6 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }.offset(i1 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i2 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 201 as i32,
// CHECK-NEXT:                 c"(*pa1)[i1] + i2".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }
// CHECK-NEXT:                                 .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }.offset(i2 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i3 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((8 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 202 as i32,
// CHECK-NEXT:                 c"(*pa1)[i2] + i3".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { (*unsafe { pa1 }).as_ptr() as *const [i8; 28] }
// CHECK-NEXT:                                 .offset(i2 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i3 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 8 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 204 as i32,
// CHECK-NEXT:                 c"*(*(paa[0]) + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 *unsafe {
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 205 as i32,
// CHECK-NEXT:                 c"*(*(paa[0]) + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 *unsafe {
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i2 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 206 as i32,
// CHECK-NEXT:                 c"*(*(paa[0]) + i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 *unsafe {
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i2 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i0 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 208 as i32,
// CHECK-NEXT:                 c"*(*(paa[1]) + i0)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 *unsafe {
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                     .offset((1 as i32) as isize)
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i0 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i1 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((9 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 209 as i32,
// CHECK-NEXT:                 c"*(*(paa[1]) + i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 *unsafe {
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                     .offset((1 as i32) as isize)
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i1 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(i2 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 210 as i32,
// CHECK-NEXT:                 c"*(*(paa[1]) + i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 *unsafe {
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                     .offset((1 as i32) as isize)
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(i2 as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i1 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 212 as i32,
// CHECK-NEXT:                 c"*(*(paa[1]) - i1)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 *unsafe {
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                     .offset((1 as i32) as isize)
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i1 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 5 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i2 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 213 as i32,
// CHECK-NEXT:                 c"*(*(paa[1]) - i2)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 *unsafe {
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                     .offset((1 as i32) as isize)
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i2 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset(-(i3 as isize))
// CHECK-NEXT:             })
// CHECK-NEXT:             .as_ptr() as *const i8
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 214 as i32,
// CHECK-NEXT:                 c"*(*(paa[1]) - i3)".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 *unsafe {
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                     .offset((1 as i32) as isize)
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(-(i3 as isize))
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_ptr() as *const i8
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             *unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i0 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i1 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 216 as i32,
// CHECK-NEXT:                 c"*(*(paa[0]) + i0) + i1".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     *unsafe {
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i0 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             *unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i1 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i2 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 217 as i32,
// CHECK-NEXT:                 c"*(*(paa[0]) + i1) + i2".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     *unsafe {
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i1 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen(unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             *unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3] }
// CHECK-NEXT:                                     .offset((0 as i32) as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i2 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_ptr() as *const i8
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset(i3 as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == (((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"line %i: strlen (%s = \"%s\") != %i\n".as_ptr() as *const i8,
// CHECK-NEXT:                 218 as i32,
// CHECK-NEXT:                 c"*(*(paa[0]) + i2) + i3".as_ptr() as *mut i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 (*unsafe {
// CHECK-NEXT:                                     *unsafe {
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             std::ptr::addr_of!(paa.0) as *const *const {{\[\[}}i8; 28]; 3]
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                         .offset((0 as i32) as isize)
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                 .as_ptr() as *const [i8; 28]
// CHECK-NEXT:                             }
// CHECK-NEXT:                             .offset(i2 as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_ptr() as *const i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset(i3 as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 2 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test_array_ptr();
// CHECK-NEXT:     test_ptr_array();
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
