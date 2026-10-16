#define vector(elcount, type)                                                  \
  __attribute__((vector_size((elcount) * sizeof(type)))) type

#define vidx(type, vec, idx) (*((type *)&(vec) + idx))

#define veccompare(type, count, v0, v1)                                        \
  do {                                                                         \
    int __i;                                                                   \
    for (__i = 0; __i < count; __i++) {                                        \
      if (vidx(type, v0, __i) != vidx(type, v1, __i))                          \
        __builtin_abort();                                                     \
    }                                                                          \
  } while (0)

int main(int argc, char *argv[]) {
#define fvec_2 (vector(4, float)){2., 2., 2., 2.}
#define dvec_2 (vector(2, double)){2., 2.}

  vector(4, float) f0 = {1., 2., 3., 4.};
  vector(4, float) f1, f2;

  vector(2, double) d0 = {1., 2.};
  vector(2, double) d1, d2;

  f1 = 2 + f0;
  f2 = fvec_2 + f0;
  veccompare(float, 4, f1, f2);
  f1 = 2 - f0;
  f2 = fvec_2 - f0;
  veccompare(float, 4, f1, f2);
  f1 = 2 * f0;
  f2 = fvec_2 * f0;
  veccompare(float, 4, f1, f2);
  f1 = 2 / f0;
  f2 = fvec_2 / f0;
  veccompare(float, 4, f1, f2);

  f1 = f0 + 2;
  f2 = f0 + fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 - 2;
  f2 = f0 - fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 * 2;
  f2 = f0 * fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 / 2;
  f2 = f0 / fvec_2;
  veccompare(float, 4, f1, f2);

  d1 = 2 + d0;
  d2 = dvec_2 + d0;
  veccompare(double, 2, d1, d2);
  d1 = 2 - d0;
  d2 = dvec_2 - d0;
  veccompare(double, 2, d1, d2);
  d1 = 2 * d0;
  d2 = dvec_2 * d0;
  veccompare(double, 2, d1, d2);
  d1 = 2 / d0;
  d2 = dvec_2 / d0;
  veccompare(double, 2, d1, d2);

  d1 = d0 + 2;
  d2 = d0 + dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 - 2;
  d2 = d0 - dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 * 2;
  d2 = d0 * dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 / 2;
  d2 = d0 / dvec_2;
  veccompare(double, 2, d1, d2);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_4:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_5:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_6:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_7:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_8:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_9:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_10:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_11:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_12:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_13:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_14:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_15:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut f0: std::simd::Simd<f32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         1.0f64 as f32,
// CHECK-NEXT:         2.0f64 as f32,
// CHECK-NEXT:         3.0f64 as f32,
// CHECK-NEXT:         4.0f64 as f32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut f1: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut f2: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut d0: std::simd::Simd<f64, 2> = std::simd::Simd::from_array([1.0f64, 2.0f64]);
// CHECK-NEXT:     let mut d1: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut d2: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     f1 = std::simd::Simd::splat((2 as i32) as f32) + f0;
// CHECK-NEXT:     f2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) + f0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while __i < (4 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(f1) as *mut f32).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(f2) as *mut f32).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = std::simd::Simd::splat((2 as i32) as f32) - f0;
// CHECK-NEXT:     f2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_1]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) - f0;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while __i < (4 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(f1) as *mut f32).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(f2) as *mut f32).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = std::simd::Simd::splat((2 as i32) as f32) * f0;
// CHECK-NEXT:     f2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_2]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) * f0;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while __i < (4 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(f1) as *mut f32).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(f2) as *mut f32).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = std::simd::Simd::splat((2 as i32) as f32) / f0;
// CHECK-NEXT:     f2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_3]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:                 2.0f64 as f32,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) / f0;
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: while __i < (4 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(f1) as *mut f32).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(f2) as *mut f32).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = f0 + std::simd::Simd::splat((2 as i32) as f32);
// CHECK-NEXT:     f2 = f0
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_4]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_4]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: while __i < (4 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(f1) as *mut f32).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(f2) as *mut f32).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_9]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = f0 - std::simd::Simd::splat((2 as i32) as f32);
// CHECK-NEXT:     f2 = f0
// CHECK-NEXT:         - unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_5]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_5]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_11:__slate_break_[0-9]+]]: while __i < (4 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(f1) as *mut f32).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(f2) as *mut f32).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_10:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_11]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = f0 * std::simd::Simd::splat((2 as i32) as f32);
// CHECK-NEXT:     f2 = f0
// CHECK-NEXT:         * unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_6]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_6]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_12:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_13:__slate_break_[0-9]+]]: while __i < (4 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(f1) as *mut f32).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(f2) as *mut f32).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_12:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_13:__v[0-9]+]]: i32 = [[V_12]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_13]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = f0 / std::simd::Simd::splat((2 as i32) as f32);
// CHECK-NEXT:     f2 = f0
// CHECK-NEXT:         / unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_7]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                     2.0f64 as f32,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_7]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_14:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_15:__slate_break_[0-9]+]]: while __i < (4 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(f1) as *mut f32).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(f2) as *mut f32).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_14:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_15:__v[0-9]+]]: i32 = [[V_14]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_15]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = std::simd::Simd::splat((2 as i32) as f64) + d0;
// CHECK-NEXT:     d2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_8]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_8]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) + d0;
// CHECK-NEXT:     '[[SLATE_BREAK_16:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_17:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(d1) as *mut f64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(d2) as *mut f64).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_16:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_17:__v[0-9]+]]: i32 = [[V_16]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_17]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = std::simd::Simd::splat((2 as i32) as f64) - d0;
// CHECK-NEXT:     d2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_9]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_9]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) - d0;
// CHECK-NEXT:     '[[SLATE_BREAK_18:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_19:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(d1) as *mut f64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(d2) as *mut f64).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_18:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_19:__v[0-9]+]]: i32 = [[V_18]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_19]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = std::simd::Simd::splat((2 as i32) as f64) * d0;
// CHECK-NEXT:     d2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_10]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_10]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) * d0;
// CHECK-NEXT:     '[[SLATE_BREAK_20:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_21:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(d1) as *mut f64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(d2) as *mut f64).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_20:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_21:__v[0-9]+]]: i32 = [[V_20]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_21]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = std::simd::Simd::splat((2 as i32) as f64) / d0;
// CHECK-NEXT:     d2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_11]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_11]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) / d0;
// CHECK-NEXT:     '[[SLATE_BREAK_22:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_23:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(d1) as *mut f64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(d2) as *mut f64).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_22:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_23:__v[0-9]+]]: i32 = [[V_22]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_23]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = d0 + std::simd::Simd::splat((2 as i32) as f64);
// CHECK-NEXT:     d2 = d0
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_12]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_12]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_24:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_25:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(d1) as *mut f64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(d2) as *mut f64).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_24:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_25:__v[0-9]+]]: i32 = [[V_24]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_25]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = d0 - std::simd::Simd::splat((2 as i32) as f64);
// CHECK-NEXT:     d2 = d0
// CHECK-NEXT:         - unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_13]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_13]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_26:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_27:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(d1) as *mut f64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(d2) as *mut f64).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_26:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_27:__v[0-9]+]]: i32 = [[V_26]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_27]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = d0 * std::simd::Simd::splat((2 as i32) as f64);
// CHECK-NEXT:     d2 = d0
// CHECK-NEXT:         * unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_14]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_14]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_28:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_29:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(d1) as *mut f64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(d2) as *mut f64).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_28:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_29:__v[0-9]+]]: i32 = [[V_28]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_29]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = d0 / std::simd::Simd::splat((2 as i32) as f64);
// CHECK-NEXT:     d2 = d0
// CHECK-NEXT:         / unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_15]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_15]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_30:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_31:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(d1) as *mut f64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(d2) as *mut f64).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_30:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_31:__v[0-9]+]]: i32 = [[V_30]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_31]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
