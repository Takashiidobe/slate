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

long __attribute__((noinline))  vlng() { return (long)42; }
int __attribute__((noinline))   vint() { return (int)43; }
short __attribute__((noinline)) vsrt() { return (short)42; }
char __attribute__((noinline))  vchr() { return (char)42; }

int main(int argc, char *argv[]) {
  vector(16, char) c0 = {argc, 1, 2, 3, 4, 5, 6, 7, argc, 1, 2, 3, 4, 5, 6, 7};
  vector(16, char) c1;

  vector(8, short) s0 = {argc, 1, 2, 3, 4, 5, 6, 7};
  vector(8, short) s1;

  vector(4, int) i0 = {argc, 1, 2, 3};
  vector(4, int) i1;

  vector(2, long) l0 = {argc, 1};
  vector(2, long) l1;

  c1 = vchr() + c0;
  check(char, 16, c0, c1, vchr(), +, l);

  s1 = vsrt() + s0;
  check(short, 8, s0, s1, vsrt(), +, l);
  s1 = vchr() + s0;
  check(short, 8, s0, s1, vchr(), +, l);

  i1 = vint() * i0;
  check(int, 4, i0, i1, vint(), *, l);
  i1 = vsrt() * i0;
  check(int, 4, i0, i1, vsrt(), *, l);
  i1 = vchr() * i0;
  check(int, 4, i0, i1, vchr(), *, l);

  l1 = vlng() * l0;
  check(long, 2, l0, l1, vlng(), *, l);
  l1 = vint() * l0;
  check(long, 2, l0, l1, vint(), *, l);
  l1 = vsrt() * l0;
  check(long, 2, l0, l1, vsrt(), *, l);
  l1 = vchr() * l0;
  check(long, 2, l0, l1, vchr(), *, l);

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
// CHECK-NEXT: fn vlng() -> i64 {
// CHECK-NEXT:     (42 as i32) as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn vint() -> i32 {
// CHECK-NEXT:     43 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn vsrt() -> i16 {
// CHECK-NEXT:     (42 as i32) as i16
// CHECK-NEXT: }
// CHECK-NEXT: fn vchr() -> i8 {
// CHECK-NEXT:     (42 as i32) as i8
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut c0: std::simd::Simd<i8, 16> = std::simd::Simd::from_array([
// CHECK-NEXT:         argc as i8,
// CHECK-NEXT:         (1 as i32) as i8,
// CHECK-NEXT:         (2 as i32) as i8,
// CHECK-NEXT:         (3 as i32) as i8,
// CHECK-NEXT:         (4 as i32) as i8,
// CHECK-NEXT:         (5 as i32) as i8,
// CHECK-NEXT:         (6 as i32) as i8,
// CHECK-NEXT:         (7 as i32) as i8,
// CHECK-NEXT:         argc as i8,
// CHECK-NEXT:         (1 as i32) as i8,
// CHECK-NEXT:         (2 as i32) as i8,
// CHECK-NEXT:         (3 as i32) as i8,
// CHECK-NEXT:         (4 as i32) as i8,
// CHECK-NEXT:         (5 as i32) as i8,
// CHECK-NEXT:         (6 as i32) as i8,
// CHECK-NEXT:         (7 as i32) as i8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut c1: std::simd::Simd<i8, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut s0: std::simd::Simd<i16, 8> = std::simd::Simd::from_array([
// CHECK-NEXT:         argc as i16,
// CHECK-NEXT:         (1 as i32) as i16,
// CHECK-NEXT:         (2 as i32) as i16,
// CHECK-NEXT:         (3 as i32) as i16,
// CHECK-NEXT:         (4 as i32) as i16,
// CHECK-NEXT:         (5 as i32) as i16,
// CHECK-NEXT:         (6 as i32) as i16,
// CHECK-NEXT:         (7 as i32) as i16,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut s1: std::simd::Simd<i16, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i0: std::simd::Simd<i32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         argc,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut i1: std::simd::Simd<i32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut l0: std::simd::Simd<i64, 2> = std::simd::Simd::from_array([
// CHECK-NEXT:         argc as i64,
// CHECK-NEXT:         (1 as i32) as i64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut l1: std::simd::Simd<i64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     c1 = std::simd::Simd::splat((vchr() as i32) as i8) + c0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while __i < (16 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(c1) as *mut i8).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (vchr() as i32)
// CHECK-NEXT:                     + ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(c0) as *mut i8).offset(__i as isize)
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
// CHECK-NEXT:     s1 = std::simd::Simd::splat((vsrt() as i32) as i16) + s0;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(s1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (vsrt() as i32)
// CHECK-NEXT:                     + ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(s0) as *mut i16).offset(__i as isize)
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
// CHECK-NEXT:     s1 = std::simd::Simd::splat((vchr() as i32) as i16) + s0;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while __i < (8 as i32) {
// CHECK-NEXT:             if ((unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(s1) as *mut i16).offset(__i as isize) }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (vchr() as i32)
// CHECK-NEXT:                     + ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(s0) as *mut i16).offset(__i as isize)
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
// CHECK-NEXT:     i1 = std::simd::Simd::splat(vint()) * i0;
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: while __i < (4 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(i1) as *mut i32).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != vint()
// CHECK-NEXT:                     * unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(i0) as *mut i32).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
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
// CHECK-NEXT:     i1 = std::simd::Simd::splat(vsrt() as i32) * i0;
// CHECK-NEXT:     '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: while __i < (4 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(i1) as *mut i32).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != (vsrt() as i32)
// CHECK-NEXT:                     * unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(i0) as *mut i32).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
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
// CHECK-NEXT:     i1 = std::simd::Simd::splat(vchr() as i32) * i0;
// CHECK-NEXT:     '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_11:__slate_break_[0-9]+]]: while __i < (4 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(i1) as *mut i32).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != (vchr() as i32)
// CHECK-NEXT:                     * unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(i0) as *mut i32).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
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
// CHECK-NEXT:     l1 = std::simd::Simd::splat(vlng()) * l0;
// CHECK-NEXT:     '[[SLATE_BREAK_12:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_13:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(l1) as *mut i64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != vlng()
// CHECK-NEXT:                     * unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(l0) as *mut i64).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
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
// CHECK-NEXT:     l1 = std::simd::Simd::splat(vint() as i64) * l0;
// CHECK-NEXT:     '[[SLATE_BREAK_14:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_15:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(l1) as *mut i64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != (vint() as i64)
// CHECK-NEXT:                     * unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(l0) as *mut i64).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
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
// CHECK-NEXT:     l1 = std::simd::Simd::splat((vsrt() as i32) as i64) * l0;
// CHECK-NEXT:     '[[SLATE_BREAK_16:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_17:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(l1) as *mut i64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != ((vsrt() as i32) as i64)
// CHECK-NEXT:                     * unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(l0) as *mut i64).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
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
// CHECK-NEXT:     l1 = std::simd::Simd::splat((vchr() as i32) as i64) * l0;
// CHECK-NEXT:     '[[SLATE_BREAK_18:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut __i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_19:__slate_break_[0-9]+]]: while __i < (2 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (std::ptr::addr_of_mut!(l1) as *mut i64).offset(__i as isize) }
// CHECK-NEXT:             })
// CHECK-NEXT:                 != ((vchr() as i32) as i64)
// CHECK-NEXT:                     * unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (std::ptr::addr_of_mut!(l0) as *mut i64).offset(__i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
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
