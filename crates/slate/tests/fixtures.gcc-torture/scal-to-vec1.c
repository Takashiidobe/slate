#define vector(elcount, type)                                                  \
  __attribute__((vector_size((elcount) * sizeof(type)))) type

#define vidx(type, vec, idx) (*((type *)&(vec) + idx))

#define operl(a, b, op) (a op b)
#define operr(a, b, op) (b op a)

#define check(type, count, vec0, vec1, num, op, lr)                            \
  do {                                                                         \
    int __i;                                                                   \
    for (__i = 0; __i < count; __i++) {                                        \
      if (vidx(type, vec1, __i) != oper##lr(num, vidx(type, vec0, __i), op))   \
        __builtin_abort();                                                     \
    }                                                                          \
  } while (0)

#define veccompare(type, count, v0, v1)                                        \
  do {                                                                         \
    int __i;                                                                   \
    for (__i = 0; __i < count; __i++) {                                        \
      if (vidx(type, v0, __i) != vidx(type, v1, __i))                          \
        __builtin_abort();                                                     \
    }                                                                          \
  } while (0)

volatile int one = 1;

int main(int argc, char *argv[]) {
#define fvec_2 (vector(4, float)){2., 2., 2., 2.}
#define dvec_2 (vector(2, double)){2., 2.}

  vector(8, short) v0 = {one, 1, 2, 3, 4, 5, 6, 7};
  vector(8, short) v1;

  vector(4, float) f0 = {1., 2., 3., 4.};
  vector(4, float) f1, f2;

  vector(2, double) d0 = {1., 2.};
  vector(2, double) d1, d2;

  v1 = 2 + v0;
  check(short, 8, v0, v1, 2, +, l);
  v1 = 2 - v0;
  check(short, 8, v0, v1, 2, -, l);
  v1 = 2 * v0;
  check(short, 8, v0, v1, 2, *, l);
  v1 = 2 / v0;
  check(short, 8, v0, v1, 2, /, l);
  v1 = 2 % v0;
  check(short, 8, v0, v1, 2, %, l);
  v1 = 2 ^ v0;
  check(short, 8, v0, v1, 2, ^, l);
  v1 = 2 & v0;
  check(short, 8, v0, v1, 2, &, l);
  v1 = 2 | v0;
  check(short, 8, v0, v1, 2, |, l);
  v1 = 2 << v0;
  check(short, 8, v0, v1, 2, <<, l);
  v1 = 2 >> v0;
  check(short, 8, v0, v1, 2, >>, l);

  v1 = v0 + 2;
  check(short, 8, v0, v1, 2, +, r);
  v1 = v0 - 2;
  check(short, 8, v0, v1, 2, -, r);
  v1 = v0 * 2;
  check(short, 8, v0, v1, 2, *, r);
  v1 = v0 / 2;
  check(short, 8, v0, v1, 2, /, r);
  v1 = v0 % 2;
  check(short, 8, v0, v1, 2, %, r);
  v1 = v0 ^ 2;
  check(short, 8, v0, v1, 2, ^, r);
  v1 = v0 & 2;
  check(short, 8, v0, v1, 2, &, r);
  v1 = v0 | 2;
  check(short, 8, v0, v1, 2, |, r);

  f1 = 2. + f0;
  f2 = fvec_2 + f0;
  veccompare(float, 4, f1, f2);
  f1 = 2. - f0;
  f2 = fvec_2 - f0;
  veccompare(float, 4, f1, f2);
  f1 = 2. * f0;
  f2 = fvec_2 * f0;
  veccompare(float, 4, f1, f2);
  f1 = 2. / f0;
  f2 = fvec_2 / f0;
  veccompare(float, 4, f1, f2);

  f1 = f0 + 2.;
  f2 = f0 + fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 - 2.;
  f2 = f0 - fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 * 2.;
  f2 = f0 * fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 / 2.;
  f2 = f0 / fvec_2;
  veccompare(float, 4, f1, f2);

  d1 = 2. + d0;
  d2 = dvec_2 + d0;
  veccompare(double, 2, d1, d2);
  d1 = 2. - d0;
  d2 = dvec_2 - d0;
  veccompare(double, 2, d1, d2);
  d1 = 2. * d0;
  d2 = dvec_2 * d0;
  veccompare(double, 2, d1, d2);
  d1 = 2. / d0;
  d2 = dvec_2 / d0;
  veccompare(double, 2, d1, d2);

  d1 = d0 + 2.;
  d2 = d0 + dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 - 2.;
  d2 = d0 - dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 * 2.;
  d2 = d0 * dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 / 2.;
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
// CHECK-NEXT: static mut one: i32 = 1 as i32;
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_4:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_5:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_6:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_7:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_8:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_9:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_10:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_11:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_12:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_13:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_14:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_15:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut v0: std::simd::Simd<i16, 8> = std::simd::Simd::from_array([
// CHECK-NEXT:         (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) }) as i16,
// CHECK-NEXT:         (1 as i32) as i16,
// CHECK-NEXT:         (2 as i32) as i16,
// CHECK-NEXT:         (3 as i32) as i16,
// CHECK-NEXT:         (4 as i32) as i16,
// CHECK-NEXT:         (5 as i32) as i16,
// CHECK-NEXT:         (6 as i32) as i16,
// CHECK-NEXT:         (7 as i32) as i16,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut v1: std::simd::Simd<i16, 8> = unsafe { std::mem::zeroed() };
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
// CHECK-NEXT:     v1 = std::simd::Simd::splat((2 as i32) as i16) + v0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (2 as i32)
// CHECK-NEXT:                     + ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as i32)
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
// CHECK-NEXT:     v1 = std::simd::Simd::splat((2 as i32) as i16) - v0;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (2 as i32)
// CHECK-NEXT:                     - ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as i32)
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
// CHECK-NEXT:     v1 = std::simd::Simd::splat((2 as i32) as i16) * v0;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (2 as i32)
// CHECK-NEXT:                     * ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as i32)
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
// CHECK-NEXT:     v1 = std::simd::Simd::splat((2 as i32) as i16) / v0;
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (2 as i32)
// CHECK-NEXT:                     / ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as i32)
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
// CHECK-NEXT:     v1 = std::simd::Simd::splat((2 as i32) as i16) % v0;
// CHECK-NEXT:     '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (2 as i32)
// CHECK-NEXT:                     % ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as i32)
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
// CHECK-NEXT:     v1 = std::simd::Simd::splat((2 as i32) as i16) ^ v0;
// CHECK-NEXT:     '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_11:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (2 as i32)
// CHECK-NEXT:                     ^ ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as i32)
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
// CHECK-NEXT:     v1 = std::simd::Simd::splat((2 as i32) as i16) & v0;
// CHECK-NEXT:     '[[SLATE_BREAK_12:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_13:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (2 as i32)
// CHECK-NEXT:                     & ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as i32)
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
// CHECK-NEXT:     v1 = std::simd::Simd::splat((2 as i32) as i16) | v0;
// CHECK-NEXT:     '[[SLATE_BREAK_14:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_15:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (2 as i32)
// CHECK-NEXT:                     | ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as i32)
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
// CHECK-NEXT:     v1 = std::simd::Simd::splat((2 as i32) as i16) << v0;
// CHECK-NEXT:     '[[SLATE_BREAK_16:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_17:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (2 as i32)
// CHECK-NEXT:                     << ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as i32)
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
// CHECK-NEXT:     v1 = std::simd::Simd::splat((2 as i32) as i16) >> v0;
// CHECK-NEXT:     '[[SLATE_BREAK_18:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_19:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (2 as i32)
// CHECK-NEXT:                     >> ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as i32)
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
// CHECK-NEXT:     v1 = v0 + std::simd::Simd::splat((2 as i32) as i16);
// CHECK-NEXT:     '[[SLATE_BREAK_20:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_21:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != ((unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as i32) + (2 as i32)
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
// CHECK-NEXT:     v1 = v0 - std::simd::Simd::splat((2 as i32) as i16);
// CHECK-NEXT:     '[[SLATE_BREAK_22:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_23:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != ((unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as i32) - (2 as i32)
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
// CHECK-NEXT:     v1 = v0 * std::simd::Simd::splat((2 as i32) as i16);
// CHECK-NEXT:     '[[SLATE_BREAK_24:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_25:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != ((unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as i32) * (2 as i32)
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
// CHECK-NEXT:     v1 = v0 / std::simd::Simd::splat((2 as i32) as i16);
// CHECK-NEXT:     '[[SLATE_BREAK_26:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_27:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != ((unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as i32) / (2 as i32)
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
// CHECK-NEXT:     v1 = v0 % std::simd::Simd::splat((2 as i32) as i16);
// CHECK-NEXT:     '[[SLATE_BREAK_28:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_29:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != ((unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as i32) % (2 as i32)
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
// CHECK-NEXT:     v1 = v0 ^ std::simd::Simd::splat((2 as i32) as i16);
// CHECK-NEXT:     '[[SLATE_BREAK_30:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_31:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != ((unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as i32) ^ (2 as i32)
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
// CHECK-NEXT:     v1 = v0 & std::simd::Simd::splat((2 as i32) as i16);
// CHECK-NEXT:     '[[SLATE_BREAK_32:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_33:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != ((unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as i32) & (2 as i32)
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_32:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_33:__v[0-9]+]]: i32 = [[V_32]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_33]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     v1 = v0 | std::simd::Simd::splat((2 as i32) as i16);
// CHECK-NEXT:     '[[SLATE_BREAK_34:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_35:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(v1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != ((unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(v0) as *mut i16).offset(__i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as i32) | (2 as i32)
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_34:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_35:__v[0-9]+]]: i32 = [[V_34]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_35]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = std::simd::Simd::splat(2.0f64 as f32) + f0;
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
// CHECK-NEXT:     '[[SLATE_BREAK_36:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_37:__slate_break_[0-9]+]]: while __i < (4 as i32) {
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
// CHECK-NEXT:             let mut [[V_36:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_37:__v[0-9]+]]: i32 = [[V_36]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_37]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = std::simd::Simd::splat(2.0f64 as f32) - f0;
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
// CHECK-NEXT:     '[[SLATE_BREAK_38:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_39:__slate_break_[0-9]+]]: while __i < (4 as i32) {
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
// CHECK-NEXT:             let mut [[V_38:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_39:__v[0-9]+]]: i32 = [[V_38]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_39]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = std::simd::Simd::splat(2.0f64 as f32) * f0;
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
// CHECK-NEXT:     '[[SLATE_BREAK_40:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_41:__slate_break_[0-9]+]]: while __i < (4 as i32) {
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
// CHECK-NEXT:             let mut [[V_40:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_41:__v[0-9]+]]: i32 = [[V_40]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_41]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = std::simd::Simd::splat(2.0f64 as f32) / f0;
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
// CHECK-NEXT:     '[[SLATE_BREAK_42:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_43:__slate_break_[0-9]+]]: while __i < (4 as i32) {
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
// CHECK-NEXT:             let mut [[V_42:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_43:__v[0-9]+]]: i32 = [[V_42]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_43]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = f0 + std::simd::Simd::splat(2.0f64 as f32);
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
// CHECK-NEXT:     '[[SLATE_BREAK_44:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_45:__slate_break_[0-9]+]]: while __i < (4 as i32) {
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
// CHECK-NEXT:             let mut [[V_44:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_45:__v[0-9]+]]: i32 = [[V_44]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_45]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = f0 - std::simd::Simd::splat(2.0f64 as f32);
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
// CHECK-NEXT:     '[[SLATE_BREAK_46:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_47:__slate_break_[0-9]+]]: while __i < (4 as i32) {
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
// CHECK-NEXT:             let mut [[V_46:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_47:__v[0-9]+]]: i32 = [[V_46]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_47]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = f0 * std::simd::Simd::splat(2.0f64 as f32);
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
// CHECK-NEXT:     '[[SLATE_BREAK_48:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_49:__slate_break_[0-9]+]]: while __i < (4 as i32) {
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
// CHECK-NEXT:             let mut [[V_48:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_49:__v[0-9]+]]: i32 = [[V_48]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_49]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1 = f0 / std::simd::Simd::splat(2.0f64 as f32);
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
// CHECK-NEXT:     '[[SLATE_BREAK_50:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_51:__slate_break_[0-9]+]]: while __i < (4 as i32) {
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
// CHECK-NEXT:             let mut [[V_50:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_51:__v[0-9]+]]: i32 = [[V_50]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_51]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = std::simd::Simd::splat(2.0f64) + d0;
// CHECK-NEXT:     d2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_8]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_8]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) + d0;
// CHECK-NEXT:     '[[SLATE_BREAK_52:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_53:__slate_break_[0-9]+]]: while __i < (2 as i32) {
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
// CHECK-NEXT:             let mut [[V_52:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_53:__v[0-9]+]]: i32 = [[V_52]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_53]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = std::simd::Simd::splat(2.0f64) - d0;
// CHECK-NEXT:     d2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_9]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_9]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) - d0;
// CHECK-NEXT:     '[[SLATE_BREAK_54:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_55:__slate_break_[0-9]+]]: while __i < (2 as i32) {
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
// CHECK-NEXT:             let mut [[V_54:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_55:__v[0-9]+]]: i32 = [[V_54]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_55]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = std::simd::Simd::splat(2.0f64) * d0;
// CHECK-NEXT:     d2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_10]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_10]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) * d0;
// CHECK-NEXT:     '[[SLATE_BREAK_56:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_57:__slate_break_[0-9]+]]: while __i < (2 as i32) {
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
// CHECK-NEXT:             let mut [[V_56:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_57:__v[0-9]+]]: i32 = [[V_56]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_57]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = std::simd::Simd::splat(2.0f64) / d0;
// CHECK-NEXT:     d2 = (unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_11]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_11]])
// CHECK-NEXT:         }
// CHECK-NEXT:     }) / d0;
// CHECK-NEXT:     '[[SLATE_BREAK_58:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_59:__slate_break_[0-9]+]]: while __i < (2 as i32) {
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
// CHECK-NEXT:             let mut [[V_58:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_59:__v[0-9]+]]: i32 = [[V_58]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_59]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = d0 + std::simd::Simd::splat(2.0f64);
// CHECK-NEXT:     d2 = d0
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_12]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_12]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_60:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_61:__slate_break_[0-9]+]]: while __i < (2 as i32) {
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
// CHECK-NEXT:             let mut [[V_60:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_61:__v[0-9]+]]: i32 = [[V_60]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_61]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = d0 - std::simd::Simd::splat(2.0f64);
// CHECK-NEXT:     d2 = d0
// CHECK-NEXT:         - unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_13]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_13]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_62:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_63:__slate_break_[0-9]+]]: while __i < (2 as i32) {
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
// CHECK-NEXT:             let mut [[V_62:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_63:__v[0-9]+]]: i32 = [[V_62]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_63]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = d0 * std::simd::Simd::splat(2.0f64);
// CHECK-NEXT:     d2 = d0
// CHECK-NEXT:         * unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_14]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_14]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_64:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_65:__slate_break_[0-9]+]]: while __i < (2 as i32) {
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
// CHECK-NEXT:             let mut [[V_64:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_65:__v[0-9]+]]: i32 = [[V_64]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_65]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d1 = d0 / std::simd::Simd::splat(2.0f64);
// CHECK-NEXT:     d2 = d0
// CHECK-NEXT:         / unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_15]] = std::simd::Simd::from_array([2.0f64, 2.0f64]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_15]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_66:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_67:__slate_break_[0-9]+]]: while __i < (2 as i32) {
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
// CHECK-NEXT:             let mut [[V_66:__v[0-9]+]]: i32 = __i;
// CHECK-NEXT:             let mut [[V_67:__v[0-9]+]]: i32 = [[V_66]] + (1 as i32);
// CHECK-NEXT:             __i = [[V_67]];
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
