void abort(void);
void exit(int);

typedef unsigned long long ull;
volatile int               gvol[32];
ull                        gull;

#define MULTI(X)                                                               \
  X(1), X(2), X(3), X(4), X(5), X(6), X(7), X(8), X(9), X(10), X(11), X(12),   \
      X(13), X(14), X(15), X(16), X(17), X(18), X(19), X(20), X(21), X(22),    \
      X(23), X(24), X(25), X(26), X(27), X(28), X(29), X(30)

#define DECLARE(INDEX) x##INDEX
#define COPYIN(INDEX)  x##INDEX = gvol[INDEX]
#define COPYOUT(INDEX) gvol[INDEX] = x##INDEX

#define BUILD_TEST(NAME, N)                                                    \
  ull __attribute__((noinline)) NAME(int n, ull x) {                           \
    while (n--) {                                                              \
      int MULTI(DECLARE);                                                      \
      MULTI(COPYIN);                                                           \
      MULTI(COPYOUT);                                                          \
      x += N;                                                                  \
    }                                                                          \
    return x;                                                                  \
  }

#define RUN_TEST(NAME, N)                                                      \
  if (NAME(3, ~0ULL) != N * 3 - 1)                                             \
    abort();                                                                   \
  if (NAME(3, 0xffffffffULL) != N * 3 + 0xffffffffULL)                         \
    abort();

#define DO_TESTS(DO_TEST)                                                      \
  DO_TEST(t1, -2048)                                                           \
  DO_TEST(t2, -513)                                                            \
  DO_TEST(t3, -512)                                                            \
  DO_TEST(t4, -511)                                                            \
  DO_TEST(t5, -1)                                                              \
  DO_TEST(t6, 1)                                                               \
  DO_TEST(t7, 511)                                                             \
  DO_TEST(t8, 512)                                                             \
  DO_TEST(t9, 513)                                                             \
  DO_TEST(t10, gull)                                                           \
  DO_TEST(t11, -gull)

DO_TESTS(BUILD_TEST)

ull neg(ull x) { return -x; }

int main() {
  gull = 100;
  DO_TESTS(RUN_TEST)
  if (neg(gull) != -100ULL)
    abort();
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
// CHECK-NEXT: static mut gvol: __SlateAlign16<[i32; 32]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut gull: u64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn t1(mut n: i32, mut x: u64) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] - (1 as i32);
// CHECK-NEXT:         n = [[V_2]];
// CHECK-NEXT:         if !([[V_1]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut x1: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut x2: i32 = 0 as i32;
// CHECK-NEXT:         let mut x3: i32 = 0 as i32;
// CHECK-NEXT:         let mut x4: i32 = 0 as i32;
// CHECK-NEXT:         let mut x5: i32 = 0 as i32;
// CHECK-NEXT:         let mut x6: i32 = 0 as i32;
// CHECK-NEXT:         let mut x7: i32 = 0 as i32;
// CHECK-NEXT:         let mut x8: i32 = 0 as i32;
// CHECK-NEXT:         let mut x9: i32 = 0 as i32;
// CHECK-NEXT:         let mut x10: i32 = 0 as i32;
// CHECK-NEXT:         let mut x11: i32 = 0 as i32;
// CHECK-NEXT:         let mut x12: i32 = 0 as i32;
// CHECK-NEXT:         let mut x13: i32 = 0 as i32;
// CHECK-NEXT:         let mut x14: i32 = 0 as i32;
// CHECK-NEXT:         let mut x15: i32 = 0 as i32;
// CHECK-NEXT:         let mut x16: i32 = 0 as i32;
// CHECK-NEXT:         let mut x17: i32 = 0 as i32;
// CHECK-NEXT:         let mut x18: i32 = 0 as i32;
// CHECK-NEXT:         let mut x19: i32 = 0 as i32;
// CHECK-NEXT:         let mut x20: i32 = 0 as i32;
// CHECK-NEXT:         let mut x21: i32 = 0 as i32;
// CHECK-NEXT:         let mut x22: i32 = 0 as i32;
// CHECK-NEXT:         let mut x23: i32 = 0 as i32;
// CHECK-NEXT:         let mut x24: i32 = 0 as i32;
// CHECK-NEXT:         let mut x25: i32 = 0 as i32;
// CHECK-NEXT:         let mut x26: i32 = 0 as i32;
// CHECK-NEXT:         let mut x27: i32 = 0 as i32;
// CHECK-NEXT:         let mut x28: i32 = 0 as i32;
// CHECK-NEXT:         let mut x29: i32 = 0 as i32;
// CHECK-NEXT:         let mut x30: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x2 = [[V_3]];
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x3 = [[V_4]];
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x4 = [[V_5]];
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x5 = [[V_6]];
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x6 = [[V_7]];
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((7 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x7 = [[V_8]];
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((8 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x8 = [[V_9]];
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((9 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x9 = [[V_10]];
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x10 = [[V_11]];
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((11 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x11 = [[V_12]];
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((12 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x12 = [[V_13]];
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((13 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x13 = [[V_14]];
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x14 = [[V_15]];
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((15 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x15 = [[V_16]];
// CHECK-NEXT:         let mut [[V_17:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((16 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x16 = [[V_17]];
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((17 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x17 = [[V_18]];
// CHECK-NEXT:         let mut [[V_19:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((18 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x18 = [[V_19]];
// CHECK-NEXT:         let mut [[V_20:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((19 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x19 = [[V_20]];
// CHECK-NEXT:         let mut [[V_21:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((20 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x20 = [[V_21]];
// CHECK-NEXT:         let mut [[V_22:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((21 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x21 = [[V_22]];
// CHECK-NEXT:         let mut [[V_23:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((22 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x22 = [[V_23]];
// CHECK-NEXT:         let mut [[V_24:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((23 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x23 = [[V_24]];
// CHECK-NEXT:         let mut [[V_25:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((24 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x24 = [[V_25]];
// CHECK-NEXT:         let mut [[V_26:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((25 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x25 = [[V_26]];
// CHECK-NEXT:         let mut [[V_27:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((26 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x26 = [[V_27]];
// CHECK-NEXT:         let mut [[V_28:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((27 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x27 = [[V_28]];
// CHECK-NEXT:         let mut [[V_29:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((28 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x28 = [[V_29]];
// CHECK-NEXT:         let mut [[V_30:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((29 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x29 = [[V_30]];
// CHECK-NEXT:         let mut [[V_31:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x30 = [[V_31]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 x1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_32:__v[0-9]+]]: i32 = x2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_32]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_33:__v[0-9]+]]: i32 = x3;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_33]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_34:__v[0-9]+]]: i32 = x4;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_34]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_35:__v[0-9]+]]: i32 = x5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_35]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_36:__v[0-9]+]]: i32 = x6;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_36]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_37:__v[0-9]+]]: i32 = x7;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((7 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_37]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_38:__v[0-9]+]]: i32 = x8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((8 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_38]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_39:__v[0-9]+]]: i32 = x9;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((9 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_39]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_40:__v[0-9]+]]: i32 = x10;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_40]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_41:__v[0-9]+]]: i32 = x11;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((11 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_41]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_42:__v[0-9]+]]: i32 = x12;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((12 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_42]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_43:__v[0-9]+]]: i32 = x13;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((13 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_43]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_44:__v[0-9]+]]: i32 = x14;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_44]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_45:__v[0-9]+]]: i32 = x15;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((15 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_45]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_46:__v[0-9]+]]: i32 = x16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((16 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_46]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_47:__v[0-9]+]]: i32 = x17;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((17 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_47]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_48:__v[0-9]+]]: i32 = x18;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((18 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_48]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_49:__v[0-9]+]]: i32 = x19;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((19 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_49]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_50:__v[0-9]+]]: i32 = x20;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((20 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_50]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_51:__v[0-9]+]]: i32 = x21;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((21 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_51]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_52:__v[0-9]+]]: i32 = x22;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((22 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_52]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_53:__v[0-9]+]]: i32 = x23;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((23 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_53]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_54:__v[0-9]+]]: i32 = x24;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((24 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_54]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_55:__v[0-9]+]]: i32 = x25;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((25 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_55]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_56:__v[0-9]+]]: i32 = x26;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((26 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_56]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_57:__v[0-9]+]]: i32 = x27;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((27 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_57]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_58:__v[0-9]+]]: i32 = x28;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((28 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_58]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_59:__v[0-9]+]]: i32 = x29;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((29 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_59]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_60:__v[0-9]+]]: i32 = x30;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((30 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_60]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_61:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:         let mut [[V_62:__v[0-9]+]]: u64 = [[V_61]].wrapping_add((-(2048 as i32) as i64) as u64);
// CHECK-NEXT:         x = [[V_62]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn t2(mut n: i32, mut x: u64) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_63:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_64:__v[0-9]+]]: i32 = [[V_63]] - (1 as i32);
// CHECK-NEXT:         n = [[V_64]];
// CHECK-NEXT:         if !([[V_63]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut x1: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut x2: i32 = 0 as i32;
// CHECK-NEXT:         let mut x3: i32 = 0 as i32;
// CHECK-NEXT:         let mut x4: i32 = 0 as i32;
// CHECK-NEXT:         let mut x5: i32 = 0 as i32;
// CHECK-NEXT:         let mut x6: i32 = 0 as i32;
// CHECK-NEXT:         let mut x7: i32 = 0 as i32;
// CHECK-NEXT:         let mut x8: i32 = 0 as i32;
// CHECK-NEXT:         let mut x9: i32 = 0 as i32;
// CHECK-NEXT:         let mut x10: i32 = 0 as i32;
// CHECK-NEXT:         let mut x11: i32 = 0 as i32;
// CHECK-NEXT:         let mut x12: i32 = 0 as i32;
// CHECK-NEXT:         let mut x13: i32 = 0 as i32;
// CHECK-NEXT:         let mut x14: i32 = 0 as i32;
// CHECK-NEXT:         let mut x15: i32 = 0 as i32;
// CHECK-NEXT:         let mut x16: i32 = 0 as i32;
// CHECK-NEXT:         let mut x17: i32 = 0 as i32;
// CHECK-NEXT:         let mut x18: i32 = 0 as i32;
// CHECK-NEXT:         let mut x19: i32 = 0 as i32;
// CHECK-NEXT:         let mut x20: i32 = 0 as i32;
// CHECK-NEXT:         let mut x21: i32 = 0 as i32;
// CHECK-NEXT:         let mut x22: i32 = 0 as i32;
// CHECK-NEXT:         let mut x23: i32 = 0 as i32;
// CHECK-NEXT:         let mut x24: i32 = 0 as i32;
// CHECK-NEXT:         let mut x25: i32 = 0 as i32;
// CHECK-NEXT:         let mut x26: i32 = 0 as i32;
// CHECK-NEXT:         let mut x27: i32 = 0 as i32;
// CHECK-NEXT:         let mut x28: i32 = 0 as i32;
// CHECK-NEXT:         let mut x29: i32 = 0 as i32;
// CHECK-NEXT:         let mut x30: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_65:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x2 = [[V_65]];
// CHECK-NEXT:         let mut [[V_66:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x3 = [[V_66]];
// CHECK-NEXT:         let mut [[V_67:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x4 = [[V_67]];
// CHECK-NEXT:         let mut [[V_68:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x5 = [[V_68]];
// CHECK-NEXT:         let mut [[V_69:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x6 = [[V_69]];
// CHECK-NEXT:         let mut [[V_70:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((7 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x7 = [[V_70]];
// CHECK-NEXT:         let mut [[V_71:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((8 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x8 = [[V_71]];
// CHECK-NEXT:         let mut [[V_72:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((9 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x9 = [[V_72]];
// CHECK-NEXT:         let mut [[V_73:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x10 = [[V_73]];
// CHECK-NEXT:         let mut [[V_74:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((11 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x11 = [[V_74]];
// CHECK-NEXT:         let mut [[V_75:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((12 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x12 = [[V_75]];
// CHECK-NEXT:         let mut [[V_76:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((13 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x13 = [[V_76]];
// CHECK-NEXT:         let mut [[V_77:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x14 = [[V_77]];
// CHECK-NEXT:         let mut [[V_78:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((15 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x15 = [[V_78]];
// CHECK-NEXT:         let mut [[V_79:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((16 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x16 = [[V_79]];
// CHECK-NEXT:         let mut [[V_80:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((17 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x17 = [[V_80]];
// CHECK-NEXT:         let mut [[V_81:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((18 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x18 = [[V_81]];
// CHECK-NEXT:         let mut [[V_82:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((19 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x19 = [[V_82]];
// CHECK-NEXT:         let mut [[V_83:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((20 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x20 = [[V_83]];
// CHECK-NEXT:         let mut [[V_84:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((21 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x21 = [[V_84]];
// CHECK-NEXT:         let mut [[V_85:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((22 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x22 = [[V_85]];
// CHECK-NEXT:         let mut [[V_86:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((23 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x23 = [[V_86]];
// CHECK-NEXT:         let mut [[V_87:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((24 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x24 = [[V_87]];
// CHECK-NEXT:         let mut [[V_88:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((25 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x25 = [[V_88]];
// CHECK-NEXT:         let mut [[V_89:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((26 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x26 = [[V_89]];
// CHECK-NEXT:         let mut [[V_90:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((27 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x27 = [[V_90]];
// CHECK-NEXT:         let mut [[V_91:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((28 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x28 = [[V_91]];
// CHECK-NEXT:         let mut [[V_92:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((29 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x29 = [[V_92]];
// CHECK-NEXT:         let mut [[V_93:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x30 = [[V_93]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 x1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_94:__v[0-9]+]]: i32 = x2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_94]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_95:__v[0-9]+]]: i32 = x3;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_95]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_96:__v[0-9]+]]: i32 = x4;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_96]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_97:__v[0-9]+]]: i32 = x5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_97]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_98:__v[0-9]+]]: i32 = x6;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_98]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_99:__v[0-9]+]]: i32 = x7;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((7 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_99]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_100:__v[0-9]+]]: i32 = x8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((8 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_100]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_101:__v[0-9]+]]: i32 = x9;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((9 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_101]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_102:__v[0-9]+]]: i32 = x10;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_102]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_103:__v[0-9]+]]: i32 = x11;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((11 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_103]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_104:__v[0-9]+]]: i32 = x12;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((12 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_104]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_105:__v[0-9]+]]: i32 = x13;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((13 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_105]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_106:__v[0-9]+]]: i32 = x14;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_106]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_107:__v[0-9]+]]: i32 = x15;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((15 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_107]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_108:__v[0-9]+]]: i32 = x16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((16 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_108]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_109:__v[0-9]+]]: i32 = x17;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((17 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_109]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_110:__v[0-9]+]]: i32 = x18;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((18 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_110]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_111:__v[0-9]+]]: i32 = x19;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((19 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_111]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_112:__v[0-9]+]]: i32 = x20;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((20 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_112]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_113:__v[0-9]+]]: i32 = x21;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((21 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_113]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_114:__v[0-9]+]]: i32 = x22;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((22 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_114]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_115:__v[0-9]+]]: i32 = x23;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((23 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_115]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_116:__v[0-9]+]]: i32 = x24;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((24 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_116]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_117:__v[0-9]+]]: i32 = x25;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((25 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_117]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_118:__v[0-9]+]]: i32 = x26;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((26 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_118]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_119:__v[0-9]+]]: i32 = x27;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((27 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_119]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_120:__v[0-9]+]]: i32 = x28;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((28 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_120]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_121:__v[0-9]+]]: i32 = x29;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((29 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_121]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_122:__v[0-9]+]]: i32 = x30;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((30 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_122]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_123:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:         let mut [[V_124:__v[0-9]+]]: u64 = [[V_123]].wrapping_add((-(513 as i32) as i64) as u64);
// CHECK-NEXT:         x = [[V_124]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn t3(mut n: i32, mut x: u64) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_125:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_126:__v[0-9]+]]: i32 = [[V_125]] - (1 as i32);
// CHECK-NEXT:         n = [[V_126]];
// CHECK-NEXT:         if !([[V_125]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut x1: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut x2: i32 = 0 as i32;
// CHECK-NEXT:         let mut x3: i32 = 0 as i32;
// CHECK-NEXT:         let mut x4: i32 = 0 as i32;
// CHECK-NEXT:         let mut x5: i32 = 0 as i32;
// CHECK-NEXT:         let mut x6: i32 = 0 as i32;
// CHECK-NEXT:         let mut x7: i32 = 0 as i32;
// CHECK-NEXT:         let mut x8: i32 = 0 as i32;
// CHECK-NEXT:         let mut x9: i32 = 0 as i32;
// CHECK-NEXT:         let mut x10: i32 = 0 as i32;
// CHECK-NEXT:         let mut x11: i32 = 0 as i32;
// CHECK-NEXT:         let mut x12: i32 = 0 as i32;
// CHECK-NEXT:         let mut x13: i32 = 0 as i32;
// CHECK-NEXT:         let mut x14: i32 = 0 as i32;
// CHECK-NEXT:         let mut x15: i32 = 0 as i32;
// CHECK-NEXT:         let mut x16: i32 = 0 as i32;
// CHECK-NEXT:         let mut x17: i32 = 0 as i32;
// CHECK-NEXT:         let mut x18: i32 = 0 as i32;
// CHECK-NEXT:         let mut x19: i32 = 0 as i32;
// CHECK-NEXT:         let mut x20: i32 = 0 as i32;
// CHECK-NEXT:         let mut x21: i32 = 0 as i32;
// CHECK-NEXT:         let mut x22: i32 = 0 as i32;
// CHECK-NEXT:         let mut x23: i32 = 0 as i32;
// CHECK-NEXT:         let mut x24: i32 = 0 as i32;
// CHECK-NEXT:         let mut x25: i32 = 0 as i32;
// CHECK-NEXT:         let mut x26: i32 = 0 as i32;
// CHECK-NEXT:         let mut x27: i32 = 0 as i32;
// CHECK-NEXT:         let mut x28: i32 = 0 as i32;
// CHECK-NEXT:         let mut x29: i32 = 0 as i32;
// CHECK-NEXT:         let mut x30: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_127:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x2 = [[V_127]];
// CHECK-NEXT:         let mut [[V_128:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x3 = [[V_128]];
// CHECK-NEXT:         let mut [[V_129:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x4 = [[V_129]];
// CHECK-NEXT:         let mut [[V_130:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x5 = [[V_130]];
// CHECK-NEXT:         let mut [[V_131:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x6 = [[V_131]];
// CHECK-NEXT:         let mut [[V_132:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((7 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x7 = [[V_132]];
// CHECK-NEXT:         let mut [[V_133:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((8 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x8 = [[V_133]];
// CHECK-NEXT:         let mut [[V_134:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((9 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x9 = [[V_134]];
// CHECK-NEXT:         let mut [[V_135:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x10 = [[V_135]];
// CHECK-NEXT:         let mut [[V_136:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((11 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x11 = [[V_136]];
// CHECK-NEXT:         let mut [[V_137:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((12 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x12 = [[V_137]];
// CHECK-NEXT:         let mut [[V_138:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((13 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x13 = [[V_138]];
// CHECK-NEXT:         let mut [[V_139:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x14 = [[V_139]];
// CHECK-NEXT:         let mut [[V_140:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((15 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x15 = [[V_140]];
// CHECK-NEXT:         let mut [[V_141:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((16 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x16 = [[V_141]];
// CHECK-NEXT:         let mut [[V_142:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((17 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x17 = [[V_142]];
// CHECK-NEXT:         let mut [[V_143:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((18 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x18 = [[V_143]];
// CHECK-NEXT:         let mut [[V_144:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((19 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x19 = [[V_144]];
// CHECK-NEXT:         let mut [[V_145:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((20 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x20 = [[V_145]];
// CHECK-NEXT:         let mut [[V_146:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((21 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x21 = [[V_146]];
// CHECK-NEXT:         let mut [[V_147:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((22 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x22 = [[V_147]];
// CHECK-NEXT:         let mut [[V_148:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((23 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x23 = [[V_148]];
// CHECK-NEXT:         let mut [[V_149:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((24 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x24 = [[V_149]];
// CHECK-NEXT:         let mut [[V_150:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((25 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x25 = [[V_150]];
// CHECK-NEXT:         let mut [[V_151:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((26 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x26 = [[V_151]];
// CHECK-NEXT:         let mut [[V_152:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((27 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x27 = [[V_152]];
// CHECK-NEXT:         let mut [[V_153:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((28 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x28 = [[V_153]];
// CHECK-NEXT:         let mut [[V_154:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((29 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x29 = [[V_154]];
// CHECK-NEXT:         let mut [[V_155:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x30 = [[V_155]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 x1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_156:__v[0-9]+]]: i32 = x2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_156]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_157:__v[0-9]+]]: i32 = x3;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_157]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_158:__v[0-9]+]]: i32 = x4;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_158]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_159:__v[0-9]+]]: i32 = x5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_159]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_160:__v[0-9]+]]: i32 = x6;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_160]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_161:__v[0-9]+]]: i32 = x7;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((7 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_161]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_162:__v[0-9]+]]: i32 = x8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((8 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_162]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_163:__v[0-9]+]]: i32 = x9;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((9 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_163]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_164:__v[0-9]+]]: i32 = x10;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_164]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_165:__v[0-9]+]]: i32 = x11;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((11 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_165]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_166:__v[0-9]+]]: i32 = x12;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((12 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_166]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_167:__v[0-9]+]]: i32 = x13;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((13 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_167]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_168:__v[0-9]+]]: i32 = x14;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_168]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_169:__v[0-9]+]]: i32 = x15;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((15 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_169]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_170:__v[0-9]+]]: i32 = x16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((16 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_170]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_171:__v[0-9]+]]: i32 = x17;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((17 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_171]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_172:__v[0-9]+]]: i32 = x18;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((18 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_172]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_173:__v[0-9]+]]: i32 = x19;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((19 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_173]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_174:__v[0-9]+]]: i32 = x20;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((20 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_174]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_175:__v[0-9]+]]: i32 = x21;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((21 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_175]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_176:__v[0-9]+]]: i32 = x22;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((22 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_176]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_177:__v[0-9]+]]: i32 = x23;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((23 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_177]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_178:__v[0-9]+]]: i32 = x24;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((24 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_178]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_179:__v[0-9]+]]: i32 = x25;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((25 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_179]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_180:__v[0-9]+]]: i32 = x26;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((26 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_180]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_181:__v[0-9]+]]: i32 = x27;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((27 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_181]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_182:__v[0-9]+]]: i32 = x28;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((28 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_182]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_183:__v[0-9]+]]: i32 = x29;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((29 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_183]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_184:__v[0-9]+]]: i32 = x30;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((30 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_184]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_185:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:         let mut [[V_186:__v[0-9]+]]: u64 = [[V_185]].wrapping_add((-(512 as i32) as i64) as u64);
// CHECK-NEXT:         x = [[V_186]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn t4(mut n: i32, mut x: u64) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_187:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_188:__v[0-9]+]]: i32 = [[V_187]] - (1 as i32);
// CHECK-NEXT:         n = [[V_188]];
// CHECK-NEXT:         if !([[V_187]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut x1: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut x2: i32 = 0 as i32;
// CHECK-NEXT:         let mut x3: i32 = 0 as i32;
// CHECK-NEXT:         let mut x4: i32 = 0 as i32;
// CHECK-NEXT:         let mut x5: i32 = 0 as i32;
// CHECK-NEXT:         let mut x6: i32 = 0 as i32;
// CHECK-NEXT:         let mut x7: i32 = 0 as i32;
// CHECK-NEXT:         let mut x8: i32 = 0 as i32;
// CHECK-NEXT:         let mut x9: i32 = 0 as i32;
// CHECK-NEXT:         let mut x10: i32 = 0 as i32;
// CHECK-NEXT:         let mut x11: i32 = 0 as i32;
// CHECK-NEXT:         let mut x12: i32 = 0 as i32;
// CHECK-NEXT:         let mut x13: i32 = 0 as i32;
// CHECK-NEXT:         let mut x14: i32 = 0 as i32;
// CHECK-NEXT:         let mut x15: i32 = 0 as i32;
// CHECK-NEXT:         let mut x16: i32 = 0 as i32;
// CHECK-NEXT:         let mut x17: i32 = 0 as i32;
// CHECK-NEXT:         let mut x18: i32 = 0 as i32;
// CHECK-NEXT:         let mut x19: i32 = 0 as i32;
// CHECK-NEXT:         let mut x20: i32 = 0 as i32;
// CHECK-NEXT:         let mut x21: i32 = 0 as i32;
// CHECK-NEXT:         let mut x22: i32 = 0 as i32;
// CHECK-NEXT:         let mut x23: i32 = 0 as i32;
// CHECK-NEXT:         let mut x24: i32 = 0 as i32;
// CHECK-NEXT:         let mut x25: i32 = 0 as i32;
// CHECK-NEXT:         let mut x26: i32 = 0 as i32;
// CHECK-NEXT:         let mut x27: i32 = 0 as i32;
// CHECK-NEXT:         let mut x28: i32 = 0 as i32;
// CHECK-NEXT:         let mut x29: i32 = 0 as i32;
// CHECK-NEXT:         let mut x30: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_189:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x2 = [[V_189]];
// CHECK-NEXT:         let mut [[V_190:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x3 = [[V_190]];
// CHECK-NEXT:         let mut [[V_191:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x4 = [[V_191]];
// CHECK-NEXT:         let mut [[V_192:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x5 = [[V_192]];
// CHECK-NEXT:         let mut [[V_193:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x6 = [[V_193]];
// CHECK-NEXT:         let mut [[V_194:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((7 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x7 = [[V_194]];
// CHECK-NEXT:         let mut [[V_195:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((8 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x8 = [[V_195]];
// CHECK-NEXT:         let mut [[V_196:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((9 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x9 = [[V_196]];
// CHECK-NEXT:         let mut [[V_197:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x10 = [[V_197]];
// CHECK-NEXT:         let mut [[V_198:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((11 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x11 = [[V_198]];
// CHECK-NEXT:         let mut [[V_199:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((12 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x12 = [[V_199]];
// CHECK-NEXT:         let mut [[V_200:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((13 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x13 = [[V_200]];
// CHECK-NEXT:         let mut [[V_201:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x14 = [[V_201]];
// CHECK-NEXT:         let mut [[V_202:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((15 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x15 = [[V_202]];
// CHECK-NEXT:         let mut [[V_203:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((16 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x16 = [[V_203]];
// CHECK-NEXT:         let mut [[V_204:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((17 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x17 = [[V_204]];
// CHECK-NEXT:         let mut [[V_205:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((18 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x18 = [[V_205]];
// CHECK-NEXT:         let mut [[V_206:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((19 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x19 = [[V_206]];
// CHECK-NEXT:         let mut [[V_207:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((20 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x20 = [[V_207]];
// CHECK-NEXT:         let mut [[V_208:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((21 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x21 = [[V_208]];
// CHECK-NEXT:         let mut [[V_209:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((22 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x22 = [[V_209]];
// CHECK-NEXT:         let mut [[V_210:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((23 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x23 = [[V_210]];
// CHECK-NEXT:         let mut [[V_211:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((24 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x24 = [[V_211]];
// CHECK-NEXT:         let mut [[V_212:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((25 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x25 = [[V_212]];
// CHECK-NEXT:         let mut [[V_213:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((26 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x26 = [[V_213]];
// CHECK-NEXT:         let mut [[V_214:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((27 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x27 = [[V_214]];
// CHECK-NEXT:         let mut [[V_215:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((28 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x28 = [[V_215]];
// CHECK-NEXT:         let mut [[V_216:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((29 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x29 = [[V_216]];
// CHECK-NEXT:         let mut [[V_217:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x30 = [[V_217]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 x1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_218:__v[0-9]+]]: i32 = x2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_218]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_219:__v[0-9]+]]: i32 = x3;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_219]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_220:__v[0-9]+]]: i32 = x4;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_220]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_221:__v[0-9]+]]: i32 = x5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_221]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_222:__v[0-9]+]]: i32 = x6;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_222]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_223:__v[0-9]+]]: i32 = x7;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((7 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_223]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_224:__v[0-9]+]]: i32 = x8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((8 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_224]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_225:__v[0-9]+]]: i32 = x9;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((9 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_225]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_226:__v[0-9]+]]: i32 = x10;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_226]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_227:__v[0-9]+]]: i32 = x11;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((11 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_227]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_228:__v[0-9]+]]: i32 = x12;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((12 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_228]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_229:__v[0-9]+]]: i32 = x13;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((13 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_229]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_230:__v[0-9]+]]: i32 = x14;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_230]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_231:__v[0-9]+]]: i32 = x15;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((15 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_231]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_232:__v[0-9]+]]: i32 = x16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((16 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_232]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_233:__v[0-9]+]]: i32 = x17;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((17 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_233]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_234:__v[0-9]+]]: i32 = x18;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((18 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_234]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_235:__v[0-9]+]]: i32 = x19;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((19 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_235]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_236:__v[0-9]+]]: i32 = x20;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((20 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_236]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_237:__v[0-9]+]]: i32 = x21;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((21 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_237]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_238:__v[0-9]+]]: i32 = x22;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((22 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_238]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_239:__v[0-9]+]]: i32 = x23;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((23 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_239]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_240:__v[0-9]+]]: i32 = x24;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((24 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_240]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_241:__v[0-9]+]]: i32 = x25;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((25 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_241]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_242:__v[0-9]+]]: i32 = x26;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((26 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_242]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_243:__v[0-9]+]]: i32 = x27;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((27 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_243]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_244:__v[0-9]+]]: i32 = x28;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((28 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_244]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_245:__v[0-9]+]]: i32 = x29;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((29 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_245]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_246:__v[0-9]+]]: i32 = x30;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((30 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_246]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_247:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:         let mut [[V_248:__v[0-9]+]]: u64 = [[V_247]].wrapping_add((-(511 as i32) as i64) as u64);
// CHECK-NEXT:         x = [[V_248]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn t5(mut n: i32, mut x: u64) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_249:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_250:__v[0-9]+]]: i32 = [[V_249]] - (1 as i32);
// CHECK-NEXT:         n = [[V_250]];
// CHECK-NEXT:         if !([[V_249]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut x1: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut x2: i32 = 0 as i32;
// CHECK-NEXT:         let mut x3: i32 = 0 as i32;
// CHECK-NEXT:         let mut x4: i32 = 0 as i32;
// CHECK-NEXT:         let mut x5: i32 = 0 as i32;
// CHECK-NEXT:         let mut x6: i32 = 0 as i32;
// CHECK-NEXT:         let mut x7: i32 = 0 as i32;
// CHECK-NEXT:         let mut x8: i32 = 0 as i32;
// CHECK-NEXT:         let mut x9: i32 = 0 as i32;
// CHECK-NEXT:         let mut x10: i32 = 0 as i32;
// CHECK-NEXT:         let mut x11: i32 = 0 as i32;
// CHECK-NEXT:         let mut x12: i32 = 0 as i32;
// CHECK-NEXT:         let mut x13: i32 = 0 as i32;
// CHECK-NEXT:         let mut x14: i32 = 0 as i32;
// CHECK-NEXT:         let mut x15: i32 = 0 as i32;
// CHECK-NEXT:         let mut x16: i32 = 0 as i32;
// CHECK-NEXT:         let mut x17: i32 = 0 as i32;
// CHECK-NEXT:         let mut x18: i32 = 0 as i32;
// CHECK-NEXT:         let mut x19: i32 = 0 as i32;
// CHECK-NEXT:         let mut x20: i32 = 0 as i32;
// CHECK-NEXT:         let mut x21: i32 = 0 as i32;
// CHECK-NEXT:         let mut x22: i32 = 0 as i32;
// CHECK-NEXT:         let mut x23: i32 = 0 as i32;
// CHECK-NEXT:         let mut x24: i32 = 0 as i32;
// CHECK-NEXT:         let mut x25: i32 = 0 as i32;
// CHECK-NEXT:         let mut x26: i32 = 0 as i32;
// CHECK-NEXT:         let mut x27: i32 = 0 as i32;
// CHECK-NEXT:         let mut x28: i32 = 0 as i32;
// CHECK-NEXT:         let mut x29: i32 = 0 as i32;
// CHECK-NEXT:         let mut x30: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_251:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x2 = [[V_251]];
// CHECK-NEXT:         let mut [[V_252:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x3 = [[V_252]];
// CHECK-NEXT:         let mut [[V_253:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x4 = [[V_253]];
// CHECK-NEXT:         let mut [[V_254:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x5 = [[V_254]];
// CHECK-NEXT:         let mut [[V_255:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x6 = [[V_255]];
// CHECK-NEXT:         let mut [[V_256:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((7 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x7 = [[V_256]];
// CHECK-NEXT:         let mut [[V_257:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((8 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x8 = [[V_257]];
// CHECK-NEXT:         let mut [[V_258:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((9 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x9 = [[V_258]];
// CHECK-NEXT:         let mut [[V_259:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x10 = [[V_259]];
// CHECK-NEXT:         let mut [[V_260:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((11 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x11 = [[V_260]];
// CHECK-NEXT:         let mut [[V_261:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((12 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x12 = [[V_261]];
// CHECK-NEXT:         let mut [[V_262:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((13 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x13 = [[V_262]];
// CHECK-NEXT:         let mut [[V_263:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x14 = [[V_263]];
// CHECK-NEXT:         let mut [[V_264:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((15 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x15 = [[V_264]];
// CHECK-NEXT:         let mut [[V_265:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((16 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x16 = [[V_265]];
// CHECK-NEXT:         let mut [[V_266:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((17 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x17 = [[V_266]];
// CHECK-NEXT:         let mut [[V_267:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((18 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x18 = [[V_267]];
// CHECK-NEXT:         let mut [[V_268:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((19 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x19 = [[V_268]];
// CHECK-NEXT:         let mut [[V_269:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((20 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x20 = [[V_269]];
// CHECK-NEXT:         let mut [[V_270:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((21 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x21 = [[V_270]];
// CHECK-NEXT:         let mut [[V_271:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((22 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x22 = [[V_271]];
// CHECK-NEXT:         let mut [[V_272:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((23 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x23 = [[V_272]];
// CHECK-NEXT:         let mut [[V_273:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((24 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x24 = [[V_273]];
// CHECK-NEXT:         let mut [[V_274:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((25 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x25 = [[V_274]];
// CHECK-NEXT:         let mut [[V_275:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((26 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x26 = [[V_275]];
// CHECK-NEXT:         let mut [[V_276:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((27 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x27 = [[V_276]];
// CHECK-NEXT:         let mut [[V_277:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((28 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x28 = [[V_277]];
// CHECK-NEXT:         let mut [[V_278:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((29 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x29 = [[V_278]];
// CHECK-NEXT:         let mut [[V_279:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x30 = [[V_279]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 x1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_280:__v[0-9]+]]: i32 = x2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_280]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_281:__v[0-9]+]]: i32 = x3;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_281]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_282:__v[0-9]+]]: i32 = x4;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_282]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_283:__v[0-9]+]]: i32 = x5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_283]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_284:__v[0-9]+]]: i32 = x6;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_284]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_285:__v[0-9]+]]: i32 = x7;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((7 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_285]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_286:__v[0-9]+]]: i32 = x8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((8 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_286]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_287:__v[0-9]+]]: i32 = x9;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((9 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_287]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_288:__v[0-9]+]]: i32 = x10;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_288]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_289:__v[0-9]+]]: i32 = x11;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((11 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_289]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_290:__v[0-9]+]]: i32 = x12;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((12 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_290]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_291:__v[0-9]+]]: i32 = x13;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((13 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_291]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_292:__v[0-9]+]]: i32 = x14;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_292]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_293:__v[0-9]+]]: i32 = x15;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((15 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_293]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_294:__v[0-9]+]]: i32 = x16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((16 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_294]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_295:__v[0-9]+]]: i32 = x17;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((17 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_295]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_296:__v[0-9]+]]: i32 = x18;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((18 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_296]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_297:__v[0-9]+]]: i32 = x19;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((19 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_297]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_298:__v[0-9]+]]: i32 = x20;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((20 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_298]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_299:__v[0-9]+]]: i32 = x21;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((21 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_299]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_300:__v[0-9]+]]: i32 = x22;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((22 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_300]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_301:__v[0-9]+]]: i32 = x23;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((23 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_301]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_302:__v[0-9]+]]: i32 = x24;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((24 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_302]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_303:__v[0-9]+]]: i32 = x25;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((25 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_303]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_304:__v[0-9]+]]: i32 = x26;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((26 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_304]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_305:__v[0-9]+]]: i32 = x27;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((27 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_305]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_306:__v[0-9]+]]: i32 = x28;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((28 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_306]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_307:__v[0-9]+]]: i32 = x29;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((29 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_307]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_308:__v[0-9]+]]: i32 = x30;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((30 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_308]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_309:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:         let mut [[V_310:__v[0-9]+]]: u64 = [[V_309]].wrapping_add((-(1 as i32) as i64) as u64);
// CHECK-NEXT:         x = [[V_310]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn t6(mut n: i32, mut x: u64) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_311:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_312:__v[0-9]+]]: i32 = [[V_311]] - (1 as i32);
// CHECK-NEXT:         n = [[V_312]];
// CHECK-NEXT:         if !([[V_311]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut x1: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut x2: i32 = 0 as i32;
// CHECK-NEXT:         let mut x3: i32 = 0 as i32;
// CHECK-NEXT:         let mut x4: i32 = 0 as i32;
// CHECK-NEXT:         let mut x5: i32 = 0 as i32;
// CHECK-NEXT:         let mut x6: i32 = 0 as i32;
// CHECK-NEXT:         let mut x7: i32 = 0 as i32;
// CHECK-NEXT:         let mut x8: i32 = 0 as i32;
// CHECK-NEXT:         let mut x9: i32 = 0 as i32;
// CHECK-NEXT:         let mut x10: i32 = 0 as i32;
// CHECK-NEXT:         let mut x11: i32 = 0 as i32;
// CHECK-NEXT:         let mut x12: i32 = 0 as i32;
// CHECK-NEXT:         let mut x13: i32 = 0 as i32;
// CHECK-NEXT:         let mut x14: i32 = 0 as i32;
// CHECK-NEXT:         let mut x15: i32 = 0 as i32;
// CHECK-NEXT:         let mut x16: i32 = 0 as i32;
// CHECK-NEXT:         let mut x17: i32 = 0 as i32;
// CHECK-NEXT:         let mut x18: i32 = 0 as i32;
// CHECK-NEXT:         let mut x19: i32 = 0 as i32;
// CHECK-NEXT:         let mut x20: i32 = 0 as i32;
// CHECK-NEXT:         let mut x21: i32 = 0 as i32;
// CHECK-NEXT:         let mut x22: i32 = 0 as i32;
// CHECK-NEXT:         let mut x23: i32 = 0 as i32;
// CHECK-NEXT:         let mut x24: i32 = 0 as i32;
// CHECK-NEXT:         let mut x25: i32 = 0 as i32;
// CHECK-NEXT:         let mut x26: i32 = 0 as i32;
// CHECK-NEXT:         let mut x27: i32 = 0 as i32;
// CHECK-NEXT:         let mut x28: i32 = 0 as i32;
// CHECK-NEXT:         let mut x29: i32 = 0 as i32;
// CHECK-NEXT:         let mut x30: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_313:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x2 = [[V_313]];
// CHECK-NEXT:         let mut [[V_314:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x3 = [[V_314]];
// CHECK-NEXT:         let mut [[V_315:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x4 = [[V_315]];
// CHECK-NEXT:         let mut [[V_316:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x5 = [[V_316]];
// CHECK-NEXT:         let mut [[V_317:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x6 = [[V_317]];
// CHECK-NEXT:         let mut [[V_318:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((7 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x7 = [[V_318]];
// CHECK-NEXT:         let mut [[V_319:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((8 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x8 = [[V_319]];
// CHECK-NEXT:         let mut [[V_320:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((9 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x9 = [[V_320]];
// CHECK-NEXT:         let mut [[V_321:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x10 = [[V_321]];
// CHECK-NEXT:         let mut [[V_322:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((11 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x11 = [[V_322]];
// CHECK-NEXT:         let mut [[V_323:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((12 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x12 = [[V_323]];
// CHECK-NEXT:         let mut [[V_324:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((13 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x13 = [[V_324]];
// CHECK-NEXT:         let mut [[V_325:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x14 = [[V_325]];
// CHECK-NEXT:         let mut [[V_326:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((15 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x15 = [[V_326]];
// CHECK-NEXT:         let mut [[V_327:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((16 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x16 = [[V_327]];
// CHECK-NEXT:         let mut [[V_328:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((17 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x17 = [[V_328]];
// CHECK-NEXT:         let mut [[V_329:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((18 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x18 = [[V_329]];
// CHECK-NEXT:         let mut [[V_330:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((19 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x19 = [[V_330]];
// CHECK-NEXT:         let mut [[V_331:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((20 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x20 = [[V_331]];
// CHECK-NEXT:         let mut [[V_332:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((21 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x21 = [[V_332]];
// CHECK-NEXT:         let mut [[V_333:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((22 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x22 = [[V_333]];
// CHECK-NEXT:         let mut [[V_334:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((23 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x23 = [[V_334]];
// CHECK-NEXT:         let mut [[V_335:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((24 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x24 = [[V_335]];
// CHECK-NEXT:         let mut [[V_336:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((25 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x25 = [[V_336]];
// CHECK-NEXT:         let mut [[V_337:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((26 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x26 = [[V_337]];
// CHECK-NEXT:         let mut [[V_338:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((27 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x27 = [[V_338]];
// CHECK-NEXT:         let mut [[V_339:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((28 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x28 = [[V_339]];
// CHECK-NEXT:         let mut [[V_340:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((29 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x29 = [[V_340]];
// CHECK-NEXT:         let mut [[V_341:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x30 = [[V_341]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 x1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_342:__v[0-9]+]]: i32 = x2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_342]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_343:__v[0-9]+]]: i32 = x3;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_343]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_344:__v[0-9]+]]: i32 = x4;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_344]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_345:__v[0-9]+]]: i32 = x5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_345]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_346:__v[0-9]+]]: i32 = x6;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_346]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_347:__v[0-9]+]]: i32 = x7;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((7 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_347]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_348:__v[0-9]+]]: i32 = x8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((8 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_348]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_349:__v[0-9]+]]: i32 = x9;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((9 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_349]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_350:__v[0-9]+]]: i32 = x10;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_350]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_351:__v[0-9]+]]: i32 = x11;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((11 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_351]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_352:__v[0-9]+]]: i32 = x12;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((12 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_352]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_353:__v[0-9]+]]: i32 = x13;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((13 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_353]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_354:__v[0-9]+]]: i32 = x14;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_354]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_355:__v[0-9]+]]: i32 = x15;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((15 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_355]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_356:__v[0-9]+]]: i32 = x16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((16 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_356]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_357:__v[0-9]+]]: i32 = x17;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((17 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_357]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_358:__v[0-9]+]]: i32 = x18;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((18 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_358]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_359:__v[0-9]+]]: i32 = x19;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((19 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_359]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_360:__v[0-9]+]]: i32 = x20;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((20 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_360]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_361:__v[0-9]+]]: i32 = x21;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((21 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_361]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_362:__v[0-9]+]]: i32 = x22;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((22 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_362]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_363:__v[0-9]+]]: i32 = x23;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((23 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_363]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_364:__v[0-9]+]]: i32 = x24;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((24 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_364]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_365:__v[0-9]+]]: i32 = x25;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((25 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_365]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_366:__v[0-9]+]]: i32 = x26;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((26 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_366]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_367:__v[0-9]+]]: i32 = x27;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((27 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_367]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_368:__v[0-9]+]]: i32 = x28;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((28 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_368]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_369:__v[0-9]+]]: i32 = x29;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((29 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_369]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_370:__v[0-9]+]]: i32 = x30;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((30 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_370]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_371:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:         let mut [[V_372:__v[0-9]+]]: u64 = [[V_371]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:         x = [[V_372]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn t7(mut n: i32, mut x: u64) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_373:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_374:__v[0-9]+]]: i32 = [[V_373]] - (1 as i32);
// CHECK-NEXT:         n = [[V_374]];
// CHECK-NEXT:         if !([[V_373]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut x1: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut x2: i32 = 0 as i32;
// CHECK-NEXT:         let mut x3: i32 = 0 as i32;
// CHECK-NEXT:         let mut x4: i32 = 0 as i32;
// CHECK-NEXT:         let mut x5: i32 = 0 as i32;
// CHECK-NEXT:         let mut x6: i32 = 0 as i32;
// CHECK-NEXT:         let mut x7: i32 = 0 as i32;
// CHECK-NEXT:         let mut x8: i32 = 0 as i32;
// CHECK-NEXT:         let mut x9: i32 = 0 as i32;
// CHECK-NEXT:         let mut x10: i32 = 0 as i32;
// CHECK-NEXT:         let mut x11: i32 = 0 as i32;
// CHECK-NEXT:         let mut x12: i32 = 0 as i32;
// CHECK-NEXT:         let mut x13: i32 = 0 as i32;
// CHECK-NEXT:         let mut x14: i32 = 0 as i32;
// CHECK-NEXT:         let mut x15: i32 = 0 as i32;
// CHECK-NEXT:         let mut x16: i32 = 0 as i32;
// CHECK-NEXT:         let mut x17: i32 = 0 as i32;
// CHECK-NEXT:         let mut x18: i32 = 0 as i32;
// CHECK-NEXT:         let mut x19: i32 = 0 as i32;
// CHECK-NEXT:         let mut x20: i32 = 0 as i32;
// CHECK-NEXT:         let mut x21: i32 = 0 as i32;
// CHECK-NEXT:         let mut x22: i32 = 0 as i32;
// CHECK-NEXT:         let mut x23: i32 = 0 as i32;
// CHECK-NEXT:         let mut x24: i32 = 0 as i32;
// CHECK-NEXT:         let mut x25: i32 = 0 as i32;
// CHECK-NEXT:         let mut x26: i32 = 0 as i32;
// CHECK-NEXT:         let mut x27: i32 = 0 as i32;
// CHECK-NEXT:         let mut x28: i32 = 0 as i32;
// CHECK-NEXT:         let mut x29: i32 = 0 as i32;
// CHECK-NEXT:         let mut x30: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_375:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x2 = [[V_375]];
// CHECK-NEXT:         let mut [[V_376:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x3 = [[V_376]];
// CHECK-NEXT:         let mut [[V_377:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x4 = [[V_377]];
// CHECK-NEXT:         let mut [[V_378:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x5 = [[V_378]];
// CHECK-NEXT:         let mut [[V_379:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x6 = [[V_379]];
// CHECK-NEXT:         let mut [[V_380:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((7 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x7 = [[V_380]];
// CHECK-NEXT:         let mut [[V_381:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((8 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x8 = [[V_381]];
// CHECK-NEXT:         let mut [[V_382:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((9 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x9 = [[V_382]];
// CHECK-NEXT:         let mut [[V_383:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x10 = [[V_383]];
// CHECK-NEXT:         let mut [[V_384:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((11 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x11 = [[V_384]];
// CHECK-NEXT:         let mut [[V_385:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((12 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x12 = [[V_385]];
// CHECK-NEXT:         let mut [[V_386:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((13 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x13 = [[V_386]];
// CHECK-NEXT:         let mut [[V_387:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x14 = [[V_387]];
// CHECK-NEXT:         let mut [[V_388:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((15 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x15 = [[V_388]];
// CHECK-NEXT:         let mut [[V_389:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((16 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x16 = [[V_389]];
// CHECK-NEXT:         let mut [[V_390:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((17 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x17 = [[V_390]];
// CHECK-NEXT:         let mut [[V_391:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((18 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x18 = [[V_391]];
// CHECK-NEXT:         let mut [[V_392:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((19 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x19 = [[V_392]];
// CHECK-NEXT:         let mut [[V_393:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((20 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x20 = [[V_393]];
// CHECK-NEXT:         let mut [[V_394:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((21 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x21 = [[V_394]];
// CHECK-NEXT:         let mut [[V_395:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((22 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x22 = [[V_395]];
// CHECK-NEXT:         let mut [[V_396:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((23 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x23 = [[V_396]];
// CHECK-NEXT:         let mut [[V_397:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((24 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x24 = [[V_397]];
// CHECK-NEXT:         let mut [[V_398:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((25 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x25 = [[V_398]];
// CHECK-NEXT:         let mut [[V_399:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((26 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x26 = [[V_399]];
// CHECK-NEXT:         let mut [[V_400:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((27 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x27 = [[V_400]];
// CHECK-NEXT:         let mut [[V_401:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((28 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x28 = [[V_401]];
// CHECK-NEXT:         let mut [[V_402:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((29 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x29 = [[V_402]];
// CHECK-NEXT:         let mut [[V_403:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x30 = [[V_403]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 x1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_404:__v[0-9]+]]: i32 = x2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_404]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_405:__v[0-9]+]]: i32 = x3;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_405]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_406:__v[0-9]+]]: i32 = x4;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_406]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_407:__v[0-9]+]]: i32 = x5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_407]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_408:__v[0-9]+]]: i32 = x6;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_408]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_409:__v[0-9]+]]: i32 = x7;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((7 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_409]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_410:__v[0-9]+]]: i32 = x8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((8 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_410]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_411:__v[0-9]+]]: i32 = x9;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((9 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_411]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_412:__v[0-9]+]]: i32 = x10;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_412]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_413:__v[0-9]+]]: i32 = x11;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((11 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_413]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_414:__v[0-9]+]]: i32 = x12;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((12 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_414]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_415:__v[0-9]+]]: i32 = x13;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((13 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_415]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_416:__v[0-9]+]]: i32 = x14;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_416]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_417:__v[0-9]+]]: i32 = x15;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((15 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_417]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_418:__v[0-9]+]]: i32 = x16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((16 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_418]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_419:__v[0-9]+]]: i32 = x17;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((17 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_419]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_420:__v[0-9]+]]: i32 = x18;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((18 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_420]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_421:__v[0-9]+]]: i32 = x19;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((19 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_421]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_422:__v[0-9]+]]: i32 = x20;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((20 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_422]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_423:__v[0-9]+]]: i32 = x21;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((21 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_423]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_424:__v[0-9]+]]: i32 = x22;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((22 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_424]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_425:__v[0-9]+]]: i32 = x23;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((23 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_425]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_426:__v[0-9]+]]: i32 = x24;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((24 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_426]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_427:__v[0-9]+]]: i32 = x25;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((25 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_427]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_428:__v[0-9]+]]: i32 = x26;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((26 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_428]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_429:__v[0-9]+]]: i32 = x27;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((27 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_429]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_430:__v[0-9]+]]: i32 = x28;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((28 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_430]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_431:__v[0-9]+]]: i32 = x29;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((29 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_431]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_432:__v[0-9]+]]: i32 = x30;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((30 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_432]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_433:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:         let mut [[V_434:__v[0-9]+]]: u64 = [[V_433]].wrapping_add(((511 as i32) as i64) as u64);
// CHECK-NEXT:         x = [[V_434]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn t8(mut n: i32, mut x: u64) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_435:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_436:__v[0-9]+]]: i32 = [[V_435]] - (1 as i32);
// CHECK-NEXT:         n = [[V_436]];
// CHECK-NEXT:         if !([[V_435]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut x1: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut x2: i32 = 0 as i32;
// CHECK-NEXT:         let mut x3: i32 = 0 as i32;
// CHECK-NEXT:         let mut x4: i32 = 0 as i32;
// CHECK-NEXT:         let mut x5: i32 = 0 as i32;
// CHECK-NEXT:         let mut x6: i32 = 0 as i32;
// CHECK-NEXT:         let mut x7: i32 = 0 as i32;
// CHECK-NEXT:         let mut x8: i32 = 0 as i32;
// CHECK-NEXT:         let mut x9: i32 = 0 as i32;
// CHECK-NEXT:         let mut x10: i32 = 0 as i32;
// CHECK-NEXT:         let mut x11: i32 = 0 as i32;
// CHECK-NEXT:         let mut x12: i32 = 0 as i32;
// CHECK-NEXT:         let mut x13: i32 = 0 as i32;
// CHECK-NEXT:         let mut x14: i32 = 0 as i32;
// CHECK-NEXT:         let mut x15: i32 = 0 as i32;
// CHECK-NEXT:         let mut x16: i32 = 0 as i32;
// CHECK-NEXT:         let mut x17: i32 = 0 as i32;
// CHECK-NEXT:         let mut x18: i32 = 0 as i32;
// CHECK-NEXT:         let mut x19: i32 = 0 as i32;
// CHECK-NEXT:         let mut x20: i32 = 0 as i32;
// CHECK-NEXT:         let mut x21: i32 = 0 as i32;
// CHECK-NEXT:         let mut x22: i32 = 0 as i32;
// CHECK-NEXT:         let mut x23: i32 = 0 as i32;
// CHECK-NEXT:         let mut x24: i32 = 0 as i32;
// CHECK-NEXT:         let mut x25: i32 = 0 as i32;
// CHECK-NEXT:         let mut x26: i32 = 0 as i32;
// CHECK-NEXT:         let mut x27: i32 = 0 as i32;
// CHECK-NEXT:         let mut x28: i32 = 0 as i32;
// CHECK-NEXT:         let mut x29: i32 = 0 as i32;
// CHECK-NEXT:         let mut x30: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_437:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x2 = [[V_437]];
// CHECK-NEXT:         let mut [[V_438:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x3 = [[V_438]];
// CHECK-NEXT:         let mut [[V_439:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x4 = [[V_439]];
// CHECK-NEXT:         let mut [[V_440:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x5 = [[V_440]];
// CHECK-NEXT:         let mut [[V_441:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x6 = [[V_441]];
// CHECK-NEXT:         let mut [[V_442:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((7 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x7 = [[V_442]];
// CHECK-NEXT:         let mut [[V_443:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((8 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x8 = [[V_443]];
// CHECK-NEXT:         let mut [[V_444:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((9 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x9 = [[V_444]];
// CHECK-NEXT:         let mut [[V_445:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x10 = [[V_445]];
// CHECK-NEXT:         let mut [[V_446:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((11 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x11 = [[V_446]];
// CHECK-NEXT:         let mut [[V_447:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((12 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x12 = [[V_447]];
// CHECK-NEXT:         let mut [[V_448:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((13 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x13 = [[V_448]];
// CHECK-NEXT:         let mut [[V_449:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x14 = [[V_449]];
// CHECK-NEXT:         let mut [[V_450:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((15 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x15 = [[V_450]];
// CHECK-NEXT:         let mut [[V_451:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((16 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x16 = [[V_451]];
// CHECK-NEXT:         let mut [[V_452:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((17 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x17 = [[V_452]];
// CHECK-NEXT:         let mut [[V_453:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((18 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x18 = [[V_453]];
// CHECK-NEXT:         let mut [[V_454:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((19 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x19 = [[V_454]];
// CHECK-NEXT:         let mut [[V_455:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((20 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x20 = [[V_455]];
// CHECK-NEXT:         let mut [[V_456:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((21 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x21 = [[V_456]];
// CHECK-NEXT:         let mut [[V_457:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((22 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x22 = [[V_457]];
// CHECK-NEXT:         let mut [[V_458:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((23 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x23 = [[V_458]];
// CHECK-NEXT:         let mut [[V_459:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((24 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x24 = [[V_459]];
// CHECK-NEXT:         let mut [[V_460:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((25 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x25 = [[V_460]];
// CHECK-NEXT:         let mut [[V_461:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((26 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x26 = [[V_461]];
// CHECK-NEXT:         let mut [[V_462:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((27 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x27 = [[V_462]];
// CHECK-NEXT:         let mut [[V_463:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((28 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x28 = [[V_463]];
// CHECK-NEXT:         let mut [[V_464:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((29 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x29 = [[V_464]];
// CHECK-NEXT:         let mut [[V_465:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x30 = [[V_465]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 x1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_466:__v[0-9]+]]: i32 = x2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_466]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_467:__v[0-9]+]]: i32 = x3;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_467]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_468:__v[0-9]+]]: i32 = x4;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_468]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_469:__v[0-9]+]]: i32 = x5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_469]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_470:__v[0-9]+]]: i32 = x6;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_470]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_471:__v[0-9]+]]: i32 = x7;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((7 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_471]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_472:__v[0-9]+]]: i32 = x8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((8 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_472]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_473:__v[0-9]+]]: i32 = x9;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((9 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_473]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_474:__v[0-9]+]]: i32 = x10;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_474]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_475:__v[0-9]+]]: i32 = x11;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((11 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_475]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_476:__v[0-9]+]]: i32 = x12;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((12 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_476]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_477:__v[0-9]+]]: i32 = x13;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((13 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_477]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_478:__v[0-9]+]]: i32 = x14;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_478]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_479:__v[0-9]+]]: i32 = x15;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((15 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_479]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_480:__v[0-9]+]]: i32 = x16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((16 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_480]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_481:__v[0-9]+]]: i32 = x17;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((17 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_481]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_482:__v[0-9]+]]: i32 = x18;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((18 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_482]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_483:__v[0-9]+]]: i32 = x19;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((19 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_483]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_484:__v[0-9]+]]: i32 = x20;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((20 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_484]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_485:__v[0-9]+]]: i32 = x21;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((21 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_485]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_486:__v[0-9]+]]: i32 = x22;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((22 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_486]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_487:__v[0-9]+]]: i32 = x23;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((23 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_487]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_488:__v[0-9]+]]: i32 = x24;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((24 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_488]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_489:__v[0-9]+]]: i32 = x25;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((25 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_489]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_490:__v[0-9]+]]: i32 = x26;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((26 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_490]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_491:__v[0-9]+]]: i32 = x27;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((27 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_491]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_492:__v[0-9]+]]: i32 = x28;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((28 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_492]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_493:__v[0-9]+]]: i32 = x29;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((29 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_493]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_494:__v[0-9]+]]: i32 = x30;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((30 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_494]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_495:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:         let mut [[V_496:__v[0-9]+]]: u64 = [[V_495]].wrapping_add(((512 as i32) as i64) as u64);
// CHECK-NEXT:         x = [[V_496]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn t9(mut n: i32, mut x: u64) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_497:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_498:__v[0-9]+]]: i32 = [[V_497]] - (1 as i32);
// CHECK-NEXT:         n = [[V_498]];
// CHECK-NEXT:         if !([[V_497]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut x1: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut x2: i32 = 0 as i32;
// CHECK-NEXT:         let mut x3: i32 = 0 as i32;
// CHECK-NEXT:         let mut x4: i32 = 0 as i32;
// CHECK-NEXT:         let mut x5: i32 = 0 as i32;
// CHECK-NEXT:         let mut x6: i32 = 0 as i32;
// CHECK-NEXT:         let mut x7: i32 = 0 as i32;
// CHECK-NEXT:         let mut x8: i32 = 0 as i32;
// CHECK-NEXT:         let mut x9: i32 = 0 as i32;
// CHECK-NEXT:         let mut x10: i32 = 0 as i32;
// CHECK-NEXT:         let mut x11: i32 = 0 as i32;
// CHECK-NEXT:         let mut x12: i32 = 0 as i32;
// CHECK-NEXT:         let mut x13: i32 = 0 as i32;
// CHECK-NEXT:         let mut x14: i32 = 0 as i32;
// CHECK-NEXT:         let mut x15: i32 = 0 as i32;
// CHECK-NEXT:         let mut x16: i32 = 0 as i32;
// CHECK-NEXT:         let mut x17: i32 = 0 as i32;
// CHECK-NEXT:         let mut x18: i32 = 0 as i32;
// CHECK-NEXT:         let mut x19: i32 = 0 as i32;
// CHECK-NEXT:         let mut x20: i32 = 0 as i32;
// CHECK-NEXT:         let mut x21: i32 = 0 as i32;
// CHECK-NEXT:         let mut x22: i32 = 0 as i32;
// CHECK-NEXT:         let mut x23: i32 = 0 as i32;
// CHECK-NEXT:         let mut x24: i32 = 0 as i32;
// CHECK-NEXT:         let mut x25: i32 = 0 as i32;
// CHECK-NEXT:         let mut x26: i32 = 0 as i32;
// CHECK-NEXT:         let mut x27: i32 = 0 as i32;
// CHECK-NEXT:         let mut x28: i32 = 0 as i32;
// CHECK-NEXT:         let mut x29: i32 = 0 as i32;
// CHECK-NEXT:         let mut x30: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_499:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x2 = [[V_499]];
// CHECK-NEXT:         let mut [[V_500:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x3 = [[V_500]];
// CHECK-NEXT:         let mut [[V_501:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x4 = [[V_501]];
// CHECK-NEXT:         let mut [[V_502:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x5 = [[V_502]];
// CHECK-NEXT:         let mut [[V_503:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x6 = [[V_503]];
// CHECK-NEXT:         let mut [[V_504:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((7 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x7 = [[V_504]];
// CHECK-NEXT:         let mut [[V_505:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((8 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x8 = [[V_505]];
// CHECK-NEXT:         let mut [[V_506:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((9 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x9 = [[V_506]];
// CHECK-NEXT:         let mut [[V_507:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x10 = [[V_507]];
// CHECK-NEXT:         let mut [[V_508:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((11 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x11 = [[V_508]];
// CHECK-NEXT:         let mut [[V_509:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((12 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x12 = [[V_509]];
// CHECK-NEXT:         let mut [[V_510:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((13 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x13 = [[V_510]];
// CHECK-NEXT:         let mut [[V_511:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x14 = [[V_511]];
// CHECK-NEXT:         let mut [[V_512:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((15 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x15 = [[V_512]];
// CHECK-NEXT:         let mut [[V_513:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((16 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x16 = [[V_513]];
// CHECK-NEXT:         let mut [[V_514:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((17 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x17 = [[V_514]];
// CHECK-NEXT:         let mut [[V_515:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((18 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x18 = [[V_515]];
// CHECK-NEXT:         let mut [[V_516:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((19 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x19 = [[V_516]];
// CHECK-NEXT:         let mut [[V_517:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((20 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x20 = [[V_517]];
// CHECK-NEXT:         let mut [[V_518:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((21 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x21 = [[V_518]];
// CHECK-NEXT:         let mut [[V_519:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((22 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x22 = [[V_519]];
// CHECK-NEXT:         let mut [[V_520:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((23 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x23 = [[V_520]];
// CHECK-NEXT:         let mut [[V_521:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((24 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x24 = [[V_521]];
// CHECK-NEXT:         let mut [[V_522:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((25 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x25 = [[V_522]];
// CHECK-NEXT:         let mut [[V_523:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((26 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x26 = [[V_523]];
// CHECK-NEXT:         let mut [[V_524:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((27 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x27 = [[V_524]];
// CHECK-NEXT:         let mut [[V_525:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((28 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x28 = [[V_525]];
// CHECK-NEXT:         let mut [[V_526:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((29 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x29 = [[V_526]];
// CHECK-NEXT:         let mut [[V_527:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x30 = [[V_527]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 x1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_528:__v[0-9]+]]: i32 = x2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_528]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_529:__v[0-9]+]]: i32 = x3;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_529]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_530:__v[0-9]+]]: i32 = x4;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_530]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_531:__v[0-9]+]]: i32 = x5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_531]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_532:__v[0-9]+]]: i32 = x6;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_532]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_533:__v[0-9]+]]: i32 = x7;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((7 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_533]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_534:__v[0-9]+]]: i32 = x8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((8 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_534]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_535:__v[0-9]+]]: i32 = x9;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((9 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_535]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_536:__v[0-9]+]]: i32 = x10;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_536]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_537:__v[0-9]+]]: i32 = x11;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((11 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_537]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_538:__v[0-9]+]]: i32 = x12;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((12 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_538]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_539:__v[0-9]+]]: i32 = x13;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((13 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_539]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_540:__v[0-9]+]]: i32 = x14;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_540]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_541:__v[0-9]+]]: i32 = x15;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((15 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_541]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_542:__v[0-9]+]]: i32 = x16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((16 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_542]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_543:__v[0-9]+]]: i32 = x17;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((17 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_543]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_544:__v[0-9]+]]: i32 = x18;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((18 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_544]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_545:__v[0-9]+]]: i32 = x19;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((19 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_545]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_546:__v[0-9]+]]: i32 = x20;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((20 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_546]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_547:__v[0-9]+]]: i32 = x21;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((21 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_547]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_548:__v[0-9]+]]: i32 = x22;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((22 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_548]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_549:__v[0-9]+]]: i32 = x23;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((23 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_549]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_550:__v[0-9]+]]: i32 = x24;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((24 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_550]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_551:__v[0-9]+]]: i32 = x25;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((25 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_551]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_552:__v[0-9]+]]: i32 = x26;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((26 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_552]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_553:__v[0-9]+]]: i32 = x27;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((27 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_553]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_554:__v[0-9]+]]: i32 = x28;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((28 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_554]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_555:__v[0-9]+]]: i32 = x29;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((29 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_555]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_556:__v[0-9]+]]: i32 = x30;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((30 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_556]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_557:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:         let mut [[V_558:__v[0-9]+]]: u64 = [[V_557]].wrapping_add(((513 as i32) as i64) as u64);
// CHECK-NEXT:         x = [[V_558]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn t10(mut n: i32, mut x: u64) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_559:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_560:__v[0-9]+]]: i32 = [[V_559]] - (1 as i32);
// CHECK-NEXT:         n = [[V_560]];
// CHECK-NEXT:         if !([[V_559]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut x1: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut x2: i32 = 0 as i32;
// CHECK-NEXT:         let mut x3: i32 = 0 as i32;
// CHECK-NEXT:         let mut x4: i32 = 0 as i32;
// CHECK-NEXT:         let mut x5: i32 = 0 as i32;
// CHECK-NEXT:         let mut x6: i32 = 0 as i32;
// CHECK-NEXT:         let mut x7: i32 = 0 as i32;
// CHECK-NEXT:         let mut x8: i32 = 0 as i32;
// CHECK-NEXT:         let mut x9: i32 = 0 as i32;
// CHECK-NEXT:         let mut x10: i32 = 0 as i32;
// CHECK-NEXT:         let mut x11: i32 = 0 as i32;
// CHECK-NEXT:         let mut x12: i32 = 0 as i32;
// CHECK-NEXT:         let mut x13: i32 = 0 as i32;
// CHECK-NEXT:         let mut x14: i32 = 0 as i32;
// CHECK-NEXT:         let mut x15: i32 = 0 as i32;
// CHECK-NEXT:         let mut x16: i32 = 0 as i32;
// CHECK-NEXT:         let mut x17: i32 = 0 as i32;
// CHECK-NEXT:         let mut x18: i32 = 0 as i32;
// CHECK-NEXT:         let mut x19: i32 = 0 as i32;
// CHECK-NEXT:         let mut x20: i32 = 0 as i32;
// CHECK-NEXT:         let mut x21: i32 = 0 as i32;
// CHECK-NEXT:         let mut x22: i32 = 0 as i32;
// CHECK-NEXT:         let mut x23: i32 = 0 as i32;
// CHECK-NEXT:         let mut x24: i32 = 0 as i32;
// CHECK-NEXT:         let mut x25: i32 = 0 as i32;
// CHECK-NEXT:         let mut x26: i32 = 0 as i32;
// CHECK-NEXT:         let mut x27: i32 = 0 as i32;
// CHECK-NEXT:         let mut x28: i32 = 0 as i32;
// CHECK-NEXT:         let mut x29: i32 = 0 as i32;
// CHECK-NEXT:         let mut x30: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_561:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x2 = [[V_561]];
// CHECK-NEXT:         let mut [[V_562:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x3 = [[V_562]];
// CHECK-NEXT:         let mut [[V_563:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x4 = [[V_563]];
// CHECK-NEXT:         let mut [[V_564:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x5 = [[V_564]];
// CHECK-NEXT:         let mut [[V_565:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x6 = [[V_565]];
// CHECK-NEXT:         let mut [[V_566:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((7 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x7 = [[V_566]];
// CHECK-NEXT:         let mut [[V_567:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((8 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x8 = [[V_567]];
// CHECK-NEXT:         let mut [[V_568:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((9 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x9 = [[V_568]];
// CHECK-NEXT:         let mut [[V_569:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x10 = [[V_569]];
// CHECK-NEXT:         let mut [[V_570:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((11 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x11 = [[V_570]];
// CHECK-NEXT:         let mut [[V_571:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((12 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x12 = [[V_571]];
// CHECK-NEXT:         let mut [[V_572:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((13 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x13 = [[V_572]];
// CHECK-NEXT:         let mut [[V_573:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x14 = [[V_573]];
// CHECK-NEXT:         let mut [[V_574:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((15 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x15 = [[V_574]];
// CHECK-NEXT:         let mut [[V_575:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((16 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x16 = [[V_575]];
// CHECK-NEXT:         let mut [[V_576:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((17 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x17 = [[V_576]];
// CHECK-NEXT:         let mut [[V_577:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((18 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x18 = [[V_577]];
// CHECK-NEXT:         let mut [[V_578:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((19 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x19 = [[V_578]];
// CHECK-NEXT:         let mut [[V_579:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((20 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x20 = [[V_579]];
// CHECK-NEXT:         let mut [[V_580:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((21 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x21 = [[V_580]];
// CHECK-NEXT:         let mut [[V_581:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((22 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x22 = [[V_581]];
// CHECK-NEXT:         let mut [[V_582:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((23 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x23 = [[V_582]];
// CHECK-NEXT:         let mut [[V_583:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((24 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x24 = [[V_583]];
// CHECK-NEXT:         let mut [[V_584:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((25 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x25 = [[V_584]];
// CHECK-NEXT:         let mut [[V_585:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((26 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x26 = [[V_585]];
// CHECK-NEXT:         let mut [[V_586:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((27 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x27 = [[V_586]];
// CHECK-NEXT:         let mut [[V_587:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((28 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x28 = [[V_587]];
// CHECK-NEXT:         let mut [[V_588:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((29 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x29 = [[V_588]];
// CHECK-NEXT:         let mut [[V_589:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x30 = [[V_589]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 x1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_590:__v[0-9]+]]: i32 = x2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_590]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_591:__v[0-9]+]]: i32 = x3;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_591]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_592:__v[0-9]+]]: i32 = x4;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_592]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_593:__v[0-9]+]]: i32 = x5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_593]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_594:__v[0-9]+]]: i32 = x6;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_594]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_595:__v[0-9]+]]: i32 = x7;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((7 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_595]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_596:__v[0-9]+]]: i32 = x8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((8 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_596]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_597:__v[0-9]+]]: i32 = x9;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((9 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_597]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_598:__v[0-9]+]]: i32 = x10;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_598]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_599:__v[0-9]+]]: i32 = x11;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((11 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_599]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_600:__v[0-9]+]]: i32 = x12;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((12 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_600]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_601:__v[0-9]+]]: i32 = x13;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((13 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_601]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_602:__v[0-9]+]]: i32 = x14;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_602]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_603:__v[0-9]+]]: i32 = x15;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((15 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_603]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_604:__v[0-9]+]]: i32 = x16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((16 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_604]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_605:__v[0-9]+]]: i32 = x17;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((17 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_605]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_606:__v[0-9]+]]: i32 = x18;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((18 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_606]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_607:__v[0-9]+]]: i32 = x19;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((19 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_607]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_608:__v[0-9]+]]: i32 = x20;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((20 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_608]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_609:__v[0-9]+]]: i32 = x21;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((21 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_609]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_610:__v[0-9]+]]: i32 = x22;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((22 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_610]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_611:__v[0-9]+]]: i32 = x23;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((23 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_611]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_612:__v[0-9]+]]: i32 = x24;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((24 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_612]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_613:__v[0-9]+]]: i32 = x25;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((25 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_613]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_614:__v[0-9]+]]: i32 = x26;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((26 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_614]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_615:__v[0-9]+]]: i32 = x27;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((27 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_615]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_616:__v[0-9]+]]: i32 = x28;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((28 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_616]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_617:__v[0-9]+]]: i32 = x29;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((29 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_617]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_618:__v[0-9]+]]: i32 = x30;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((30 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_618]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_619:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:         let mut [[V_620:__v[0-9]+]]: u64 = [[V_619]].wrapping_add(unsafe { gull });
// CHECK-NEXT:         x = [[V_620]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn t11(mut n: i32, mut x: u64) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_621:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_622:__v[0-9]+]]: i32 = [[V_621]] - (1 as i32);
// CHECK-NEXT:         n = [[V_622]];
// CHECK-NEXT:         if !([[V_621]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut x1: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut x2: i32 = 0 as i32;
// CHECK-NEXT:         let mut x3: i32 = 0 as i32;
// CHECK-NEXT:         let mut x4: i32 = 0 as i32;
// CHECK-NEXT:         let mut x5: i32 = 0 as i32;
// CHECK-NEXT:         let mut x6: i32 = 0 as i32;
// CHECK-NEXT:         let mut x7: i32 = 0 as i32;
// CHECK-NEXT:         let mut x8: i32 = 0 as i32;
// CHECK-NEXT:         let mut x9: i32 = 0 as i32;
// CHECK-NEXT:         let mut x10: i32 = 0 as i32;
// CHECK-NEXT:         let mut x11: i32 = 0 as i32;
// CHECK-NEXT:         let mut x12: i32 = 0 as i32;
// CHECK-NEXT:         let mut x13: i32 = 0 as i32;
// CHECK-NEXT:         let mut x14: i32 = 0 as i32;
// CHECK-NEXT:         let mut x15: i32 = 0 as i32;
// CHECK-NEXT:         let mut x16: i32 = 0 as i32;
// CHECK-NEXT:         let mut x17: i32 = 0 as i32;
// CHECK-NEXT:         let mut x18: i32 = 0 as i32;
// CHECK-NEXT:         let mut x19: i32 = 0 as i32;
// CHECK-NEXT:         let mut x20: i32 = 0 as i32;
// CHECK-NEXT:         let mut x21: i32 = 0 as i32;
// CHECK-NEXT:         let mut x22: i32 = 0 as i32;
// CHECK-NEXT:         let mut x23: i32 = 0 as i32;
// CHECK-NEXT:         let mut x24: i32 = 0 as i32;
// CHECK-NEXT:         let mut x25: i32 = 0 as i32;
// CHECK-NEXT:         let mut x26: i32 = 0 as i32;
// CHECK-NEXT:         let mut x27: i32 = 0 as i32;
// CHECK-NEXT:         let mut x28: i32 = 0 as i32;
// CHECK-NEXT:         let mut x29: i32 = 0 as i32;
// CHECK-NEXT:         let mut x30: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_623:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x2 = [[V_623]];
// CHECK-NEXT:         let mut [[V_624:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x3 = [[V_624]];
// CHECK-NEXT:         let mut [[V_625:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((4 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x4 = [[V_625]];
// CHECK-NEXT:         let mut [[V_626:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((5 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x5 = [[V_626]];
// CHECK-NEXT:         let mut [[V_627:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((6 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x6 = [[V_627]];
// CHECK-NEXT:         let mut [[V_628:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((7 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x7 = [[V_628]];
// CHECK-NEXT:         let mut [[V_629:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((8 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x8 = [[V_629]];
// CHECK-NEXT:         let mut [[V_630:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((9 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x9 = [[V_630]];
// CHECK-NEXT:         let mut [[V_631:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x10 = [[V_631]];
// CHECK-NEXT:         let mut [[V_632:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((11 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x11 = [[V_632]];
// CHECK-NEXT:         let mut [[V_633:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((12 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x12 = [[V_633]];
// CHECK-NEXT:         let mut [[V_634:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((13 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x13 = [[V_634]];
// CHECK-NEXT:         let mut [[V_635:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((14 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x14 = [[V_635]];
// CHECK-NEXT:         let mut [[V_636:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((15 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x15 = [[V_636]];
// CHECK-NEXT:         let mut [[V_637:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((16 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x16 = [[V_637]];
// CHECK-NEXT:         let mut [[V_638:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((17 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x17 = [[V_638]];
// CHECK-NEXT:         let mut [[V_639:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((18 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x18 = [[V_639]];
// CHECK-NEXT:         let mut [[V_640:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((19 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x19 = [[V_640]];
// CHECK-NEXT:         let mut [[V_641:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((20 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x20 = [[V_641]];
// CHECK-NEXT:         let mut [[V_642:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((21 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x21 = [[V_642]];
// CHECK-NEXT:         let mut [[V_643:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((22 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x22 = [[V_643]];
// CHECK-NEXT:         let mut [[V_644:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((23 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x23 = [[V_644]];
// CHECK-NEXT:         let mut [[V_645:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((24 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x24 = [[V_645]];
// CHECK-NEXT:         let mut [[V_646:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((25 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x25 = [[V_646]];
// CHECK-NEXT:         let mut [[V_647:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((26 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x26 = [[V_647]];
// CHECK-NEXT:         let mut [[V_648:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((27 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x27 = [[V_648]];
// CHECK-NEXT:         let mut [[V_649:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((28 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x28 = [[V_649]];
// CHECK-NEXT:         let mut [[V_650:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((29 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x29 = [[V_650]];
// CHECK-NEXT:         let mut [[V_651:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         x30 = [[V_651]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 x1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_652:__v[0-9]+]]: i32 = x2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_652]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_653:__v[0-9]+]]: i32 = x3;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((3 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_653]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_654:__v[0-9]+]]: i32 = x4;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((4 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_654]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_655:__v[0-9]+]]: i32 = x5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((5 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_655]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_656:__v[0-9]+]]: i32 = x6;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((6 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_656]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_657:__v[0-9]+]]: i32 = x7;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((7 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_657]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_658:__v[0-9]+]]: i32 = x8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((8 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_658]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_659:__v[0-9]+]]: i32 = x9;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((9 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_659]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_660:__v[0-9]+]]: i32 = x10;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((10 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_660]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_661:__v[0-9]+]]: i32 = x11;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((11 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_661]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_662:__v[0-9]+]]: i32 = x12;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((12 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_662]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_663:__v[0-9]+]]: i32 = x13;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((13 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_663]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_664:__v[0-9]+]]: i32 = x14;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((14 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_664]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_665:__v[0-9]+]]: i32 = x15;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((15 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_665]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_666:__v[0-9]+]]: i32 = x16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((16 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_666]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_667:__v[0-9]+]]: i32 = x17;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((17 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_667]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_668:__v[0-9]+]]: i32 = x18;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((18 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_668]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_669:__v[0-9]+]]: i32 = x19;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((19 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_669]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_670:__v[0-9]+]]: i32 = x20;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((20 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_670]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_671:__v[0-9]+]]: i32 = x21;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((21 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_671]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_672:__v[0-9]+]]: i32 = x22;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((22 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_672]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_673:__v[0-9]+]]: i32 = x23;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((23 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_673]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_674:__v[0-9]+]]: i32 = x24;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((24 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_674]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_675:__v[0-9]+]]: i32 = x25;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((25 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_675]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_676:__v[0-9]+]]: i32 = x26;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((26 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_676]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_677:__v[0-9]+]]: i32 = x27;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((27 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_677]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_678:__v[0-9]+]]: i32 = x28;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((28 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_678]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_679:__v[0-9]+]]: i32 = x29;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((29 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_679]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_680:__v[0-9]+]]: i32 = x30;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(gvol.0) as *mut i32 }
// CHECK-NEXT:                         .offset((30 as i32) as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 [[V_680]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_681:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:         let mut [[V_682:__v[0-9]+]]: u64 = [[V_681]].wrapping_add(unsafe { gull }.wrapping_neg());
// CHECK-NEXT:         x = [[V_682]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn neg(mut x: u64) -> u64 {
// CHECK-NEXT:     x.wrapping_neg()
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         gull = ((100 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if t1(3 as i32, !(0 as u64))
// CHECK-NEXT:         != (((-(2048 as i32) * (3 as i32) - (1 as i32)) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t1(3 as i32, 4294967295 as u64)
// CHECK-NEXT:         != (((-(2048 as i32) * (3 as i32)) as i64) as u64)
// CHECK-NEXT:             .wrapping_add(4294967295 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t2(3 as i32, !(0 as u64))
// CHECK-NEXT:         != (((-(513 as i32) * (3 as i32) - (1 as i32)) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t2(3 as i32, 4294967295 as u64)
// CHECK-NEXT:         != (((-(513 as i32) * (3 as i32)) as i64) as u64).wrapping_add(4294967295 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t3(3 as i32, !(0 as u64))
// CHECK-NEXT:         != (((-(512 as i32) * (3 as i32) - (1 as i32)) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t3(3 as i32, 4294967295 as u64)
// CHECK-NEXT:         != (((-(512 as i32) * (3 as i32)) as i64) as u64).wrapping_add(4294967295 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t4(3 as i32, !(0 as u64))
// CHECK-NEXT:         != (((-(511 as i32) * (3 as i32) - (1 as i32)) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t4(3 as i32, 4294967295 as u64)
// CHECK-NEXT:         != (((-(511 as i32) * (3 as i32)) as i64) as u64).wrapping_add(4294967295 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t5(3 as i32, !(0 as u64))
// CHECK-NEXT:         != (((-(1 as i32) * (3 as i32) - (1 as i32)) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t5(3 as i32, 4294967295 as u64)
// CHECK-NEXT:         != (((-(1 as i32) * (3 as i32)) as i64) as u64).wrapping_add(4294967295 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t6(3 as i32, !(0 as u64))
// CHECK-NEXT:         != ((((1 as i32) * (3 as i32) - (1 as i32)) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t6(3 as i32, 4294967295 as u64)
// CHECK-NEXT:         != ((((1 as i32) * (3 as i32)) as i64) as u64).wrapping_add(4294967295 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t7(3 as i32, !(0 as u64))
// CHECK-NEXT:         != ((((511 as i32) * (3 as i32) - (1 as i32)) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t7(3 as i32, 4294967295 as u64)
// CHECK-NEXT:         != ((((511 as i32) * (3 as i32)) as i64) as u64).wrapping_add(4294967295 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t8(3 as i32, !(0 as u64))
// CHECK-NEXT:         != ((((512 as i32) * (3 as i32) - (1 as i32)) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t8(3 as i32, 4294967295 as u64)
// CHECK-NEXT:         != ((((512 as i32) * (3 as i32)) as i64) as u64).wrapping_add(4294967295 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t9(3 as i32, !(0 as u64))
// CHECK-NEXT:         != ((((513 as i32) * (3 as i32) - (1 as i32)) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t9(3 as i32, 4294967295 as u64)
// CHECK-NEXT:         != ((((513 as i32) * (3 as i32)) as i64) as u64).wrapping_add(4294967295 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t10(3 as i32, !(0 as u64))
// CHECK-NEXT:         != unsafe { gull }
// CHECK-NEXT:             .wrapping_mul(((3 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t10(3 as i32, 4294967295 as u64)
// CHECK-NEXT:         != unsafe { gull }
// CHECK-NEXT:             .wrapping_mul(((3 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_add(4294967295 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t11(3 as i32, !(0 as u64))
// CHECK-NEXT:         != unsafe { gull }
// CHECK-NEXT:             .wrapping_neg()
// CHECK-NEXT:             .wrapping_mul(((3 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t11(3 as i32, 4294967295 as u64)
// CHECK-NEXT:         != unsafe { gull }
// CHECK-NEXT:             .wrapping_neg()
// CHECK-NEXT:             .wrapping_mul(((3 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_add(4294967295 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if neg(unsafe { gull }) != (100 as u64).wrapping_neg() {
// CHECK-NEXT:         unsafe { abort() };
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
