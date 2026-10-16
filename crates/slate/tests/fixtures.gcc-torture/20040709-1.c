/* { dg-require-effective-target int32plus } */

/* Test arithmetics on bitfields.  */

extern void abort(void);
extern void exit(int);

unsigned int myrnd(void) {
  static unsigned int s  = 1388815473;
  s                     *= 1103515245;
  s                     += 12345;
  return (s / 65536) % 2048;
}

#define T(S)                                                                   \
  struct S s##S;                                                               \
  struct S retme##S(struct S x) {                                              \
    return x;                                                                  \
  }                                                                            \
                                                                               \
  unsigned int                                                                 \
  fn1##S(unsigned int x) {                                                     \
    struct S y  = s##S;                                                        \
    y.k        += x;                                                           \
    y           = retme##S(y);                                                 \
    return y.k;                                                                \
  }                                                                            \
                                                                               \
  unsigned int fn2##S(unsigned int x) {                                        \
    struct S y  = s##S;                                                        \
    y.k        += x;                                                           \
    y.k        %= 15;                                                          \
    return y.k;                                                                \
  }                                                                            \
                                                                               \
  unsigned int retit##S(void) { return s##S.k; }                               \
                                                                               \
  unsigned int fn3##S(unsigned int x) {                                        \
    s##S.k += x;                                                               \
    return retit##S();                                                         \
  }                                                                            \
                                                                               \
  void test##S(void) {                                                         \
    int          i;                                                            \
    unsigned int mask, v, a, r;                                                \
    struct S     x;                                                            \
    char        *p = (char *)&s##S;                                            \
    for (i = 0; i < sizeof(s##S); ++i)                                         \
      *p++ = myrnd();                                                          \
    if (__builtin_classify_type(s##S.l) == 8)                                  \
      s##S.l = 5.25;                                                           \
    s##S.k = -1;                                                               \
    mask   = s##S.k;                                                           \
    v      = myrnd();                                                          \
    a      = myrnd();                                                          \
    s##S.k = v;                                                                \
    x      = s##S;                                                             \
    r      = fn1##S(a);                                                        \
    if (x.i != s##S.i || x.j != s##S.j || x.k != s##S.k || x.l != s##S.l ||    \
        ((v + a) & mask) != r)                                                 \
      abort();                                                                 \
    v      = myrnd();                                                          \
    a      = myrnd();                                                          \
    s##S.k = v;                                                                \
    x      = s##S;                                                             \
    r      = fn2##S(a);                                                        \
    if (x.i != s##S.i || x.j != s##S.j || x.k != s##S.k || x.l != s##S.l ||    \
        ((((v + a) & mask) % 15) & mask) != r)                                 \
      abort();                                                                 \
    v      = myrnd();                                                          \
    a      = myrnd();                                                          \
    s##S.k = v;                                                                \
    x      = s##S;                                                             \
    r      = fn3##S(a);                                                        \
    if (x.i != s##S.i || x.j != s##S.j || s##S.k != r || x.l != s##S.l ||      \
        ((v + a) & mask) != r)                                                 \
      abort();                                                                 \
  }

struct A {
  unsigned int i : 6, l : 1, j : 10, k : 15;
};
T(A)
struct B {
  unsigned int i : 6, j : 11, k : 15;
  unsigned int l;
};
T(B)
struct C {
  unsigned int l;
  unsigned int i : 6, j : 11, k : 15;
};
T(C)
struct D {
  unsigned long long l : 6, i : 6, j : 23, k : 29;
};
T(D)
struct E {
  unsigned long long l, i : 12, j : 23, k : 29;
};
T(E)
struct F {
  unsigned long long i : 12, j : 23, k : 29, l;
};
T(F)
struct G {
  unsigned int       i : 12, j : 13, k : 7;
  unsigned long long l;
};
T(G)
struct H {
  unsigned int       i : 12, j : 11, k : 9;
  unsigned long long l;
};
T(H)
struct I {
  unsigned short     i : 1, j : 6, k : 9;
  unsigned long long l;
};
T(I)
struct J {
  unsigned short i : 1, j : 8, k : 7;
  unsigned short l;
};
T(J)
struct K {
  unsigned int k : 6, l : 1, j : 10, i : 15;
};
T(K)
struct L {
  unsigned int k : 6, j : 11, i : 15;
  unsigned int l;
};
T(L)
struct M {
  unsigned int l;
  unsigned int k : 6, j : 11, i : 15;
};
T(M)
struct N {
  unsigned long long l : 6, k : 6, j : 23, i : 29;
};
T(N)
struct O {
  unsigned long long l, k : 12, j : 23, i : 29;
};
T(O)
struct P {
  unsigned long long k : 12, j : 23, i : 29, l;
};
T(P)
struct Q {
  unsigned int       k : 12, j : 13, i : 7;
  unsigned long long l;
};
T(Q)
struct R {
  unsigned int       k : 12, j : 11, i : 9;
  unsigned long long l;
};
T(R)
struct S {
  unsigned short     k : 1, j : 6, i : 9;
  unsigned long long l;
};
T(S)
struct T {
  unsigned short k : 1, j : 8, i : 7;
  unsigned short l;
};
T(T)
struct U {
  unsigned short     j : 6, k : 1, i : 9;
  unsigned long long l;
};
T(U)
struct V {
  unsigned short j : 8, k : 1, i : 7;
  unsigned short l;
};
T(V)
struct W {
  long double  l;
  unsigned int k : 12, j : 13, i : 7;
};
T(W)
struct X {
  unsigned int k : 12, j : 13, i : 7;
  long double  l;
};
T(X)
struct Y {
  unsigned int k : 12, j : 11, i : 9;
  long double  l;
};
T(Y)
struct Z {
  long double  l;
  unsigned int j : 13, i : 7, k : 12;
};
T(Z)

int main(void) {
  testA();
  testB();
  testC();
  testD();
  testE();
  testF();
  testG();
  testH();
  testI();
  testJ();
  testK();
  testL();
  testM();
  testN();
  testO();
  testP();
  testQ();
  testR();
  testS();
  testT();
  testU();
  testV();
  testW();
  testX();
  testY();
  testZ();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'_>) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// CHECK-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// CHECK-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// CHECK-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// CHECK-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// CHECK-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// CHECK-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// CHECK-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// CHECK-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// CHECK-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// CHECK-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// CHECK-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::cmp::PartialOrd for LongDouble {
// CHECK-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// CHECK-NEXT:         if __slate_f80_lt(*self, *__other) {
// CHECK-NEXT:             Some(std::cmp::Ordering::Less)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if __slate_f80_gt(*self, *__other) {
// CHECK-NEXT:                 Some(std::cmp::Ordering::Greater)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if __slate_f80_eq(*self, *__other) {
// CHECK-NEXT:                     Some(std::cmp::Ordering::Equal)
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     None
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Test arithmetics on bitfields.
// CHECK-NEXT: fn myrnd() -> u32 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = unsafe { s };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = [[V_1]].wrapping_mul((1103515245 as i32) as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u32 = unsafe { s };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u32 = [[V_3]].wrapping_add((12345 as i32) as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { s }) / ((65536 as i32) as u32) % ((2048 as i32) as u32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: u32 = (1388815473 as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sA: A = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeA(mut x: A) -> A {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1A(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: A = unsafe { sA };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u32 = (([[V_5]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_6]]);
// CHECK-NEXT:     y = retmeA(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2A(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: A = unsafe { sA };
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: u32 = (([[V_7]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_8]]);
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: u32 = (([[V_9]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_10]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitA() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3A(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: u32 = (([[V_11]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sA.__slate_bits_0)).__set_k([[V_12]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitA()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testA() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sA) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (4 as u64) {
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: *mut i8 = unsafe { [[V_13]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_14]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_13]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: i32 = [[V_15]] + (1 as i32);
// CHECK-NEXT:         i = [[V_16]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*std::ptr::addr_of_mut!(sA.__slate_bits_0)).__set_l(5.25f64 as u32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sA.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sA.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sA };
// CHECK-NEXT:     r = fn1A(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_l() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_l() }) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sA.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sA };
// CHECK-NEXT:     r = fn2A(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_l() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_l() }) as i32)
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sA.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sA };
// CHECK-NEXT:     r = fn3A(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || (x.__slate_bits_0.__get_l() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sA.__slate_bits_0)).__get_l() }) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct B {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT:     l: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sB: B = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeB(mut x: B) -> B {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1B(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: B = unsafe { sB };
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_18:__v[0-9]+]]: u32 = (([[V_17]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_18]]);
// CHECK-NEXT:     y = retmeB(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2B(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: B = unsafe { sB };
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_20:__v[0-9]+]]: u32 = (([[V_19]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_20]]);
// CHECK-NEXT:     let mut [[V_21:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_22:__v[0-9]+]]: u32 = (([[V_21]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_22]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitB() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3B(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_23:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_24:__v[0-9]+]]: u32 = (([[V_23]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sB.__slate_bits_0)).__set_k([[V_24]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitB()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testB() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: B = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sB) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         let mut [[V_25:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_26:__v[0-9]+]]: *mut i8 = unsafe { [[V_25]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_26]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_25]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_27:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_28:__v[0-9]+]]: i32 = [[V_27]] + (1 as i32);
// CHECK-NEXT:         i = [[V_28]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sB.l = 5.25f64 as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sB.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sB.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sB };
// CHECK-NEXT:     r = fn1B(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sB.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sB.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sB };
// CHECK-NEXT:     r = fn2B(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sB.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sB.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sB };
// CHECK-NEXT:     r = fn3B(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sB.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sB.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C {
// CHECK-NEXT:     l: u32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sC: C = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeC(mut x: C) -> C {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1C(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: C = unsafe { sC };
// CHECK-NEXT:     let mut [[V_29:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_30:__v[0-9]+]]: u32 = (([[V_29]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_30]]);
// CHECK-NEXT:     y = retmeC(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2C(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: C = unsafe { sC };
// CHECK-NEXT:     let mut [[V_31:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_32:__v[0-9]+]]: u32 = (([[V_31]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_32]]);
// CHECK-NEXT:     let mut [[V_33:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_34:__v[0-9]+]]: u32 = (([[V_33]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_34]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitC() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3C(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_35:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_36:__v[0-9]+]]: u32 = (([[V_35]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sC.__slate_bits_0)).__set_k([[V_36]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitC()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testC() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: C = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sC) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         let mut [[V_37:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_38:__v[0-9]+]]: *mut i8 = unsafe { [[V_37]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_38]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_37]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_39:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_40:__v[0-9]+]]: i32 = [[V_39]] + (1 as i32);
// CHECK-NEXT:         i = [[V_40]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sC.l = 5.25f64 as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sC.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sC.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sC };
// CHECK-NEXT:     r = fn1C(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sC.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sC.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sC };
// CHECK-NEXT:     r = fn2C(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sC.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sC.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sC };
// CHECK-NEXT:     r = fn3C(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sC.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sC.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub l: u32,
// CHECK-NEXT:         #[bits(10)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits2U0 {
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u64,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub l: u64,
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub i: u64,
// CHECK-NEXT:         #[bits(23)]
// CHECK-NEXT:         pub j: u64,
// CHECK-NEXT:         #[bits(29)]
// CHECK-NEXT:         pub k: u64,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u64,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits4U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub i: u64,
// CHECK-NEXT:         #[bits(23)]
// CHECK-NEXT:         pub j: u64,
// CHECK-NEXT:         #[bits(29)]
// CHECK-NEXT:         pub k: u64,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u64,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits5U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub i: u64,
// CHECK-NEXT:         #[bits(23)]
// CHECK-NEXT:         pub j: u64,
// CHECK-NEXT:         #[bits(29)]
// CHECK-NEXT:         pub k: u64,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits6U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:         #[bits(13)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits7U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(9)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits8U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub i: u16,
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub j: u16,
// CHECK-NEXT:         #[bits(9)]
// CHECK-NEXT:         pub k: u16,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits9U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub i: u16,
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub j: u16,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub k: u16,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits10U0 {
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub l: u32,
// CHECK-NEXT:         #[bits(10)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits11U0 {
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits12U0 {
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u64,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits13U0 {
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub l: u64,
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub k: u64,
// CHECK-NEXT:         #[bits(23)]
// CHECK-NEXT:         pub j: u64,
// CHECK-NEXT:         #[bits(29)]
// CHECK-NEXT:         pub i: u64,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u64,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits14U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub k: u64,
// CHECK-NEXT:         #[bits(23)]
// CHECK-NEXT:         pub j: u64,
// CHECK-NEXT:         #[bits(29)]
// CHECK-NEXT:         pub i: u64,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u64,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits15U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub k: u64,
// CHECK-NEXT:         #[bits(23)]
// CHECK-NEXT:         pub j: u64,
// CHECK-NEXT:         #[bits(29)]
// CHECK-NEXT:         pub i: u64,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits16U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:         #[bits(13)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits17U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(9)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits18U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub k: u16,
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub j: u16,
// CHECK-NEXT:         #[bits(9)]
// CHECK-NEXT:         pub i: u16,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits19U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub k: u16,
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub j: u16,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub i: u16,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits20U0 {
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub j: u16,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub k: u16,
// CHECK-NEXT:         #[bits(9)]
// CHECK-NEXT:         pub i: u16,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits21U0 {
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub j: u16,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub k: u16,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub i: u16,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits22U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:         #[bits(13)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits23U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:         #[bits(13)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits24U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(9)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits25U0 {
// CHECK-NEXT:         #[bits(13)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct D {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sD: D = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeD(mut x: D) -> D {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1D(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: D = unsafe { sD };
// CHECK-NEXT:     let mut [[V_41:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_42:__v[0-9]+]]: u64 = ((([[V_41]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_42]]);
// CHECK-NEXT:     y = retmeD(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2D(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: D = unsafe { sD };
// CHECK-NEXT:     let mut [[V_43:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_44:__v[0-9]+]]: u64 = ((([[V_43]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_44]]);
// CHECK-NEXT:     let mut [[V_45:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_46:__v[0-9]+]]: u64 = (((([[V_45]] as u32) as i32) % (15 as i32)) as i64) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_46]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitD() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3D(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_47:__v[0-9]+]]: u64 = unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_48:__v[0-9]+]]: u64 = ((([[V_47]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sD.__slate_bits_0)).__set_k([[V_48]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitD()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testD() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: D = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sD) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         let mut [[V_49:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_50:__v[0-9]+]]: *mut i8 = unsafe { [[V_49]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_50]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_49]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_51:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_52:__v[0-9]+]]: i32 = [[V_51]] + (1 as i32);
// CHECK-NEXT:         i = [[V_52]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*std::ptr::addr_of_mut!(sD.__slate_bits_0)).__set_l(5.25f64 as u64);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sD.__slate_bits_0)).__set_k((-(1 as i32) as i64) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sD.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sD };
// CHECK-NEXT:     r = fn1D(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_l() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_l() }) as u32) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sD.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sD };
// CHECK-NEXT:     r = fn2D(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_l() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_l() }) as u32) as i32)
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sD.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sD };
// CHECK-NEXT:     r = fn3D(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_l() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sD.__slate_bits_0)).__get_l() }) as u32) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct E {
// CHECK-NEXT:     l: u64,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits4U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sE: E = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeE(mut x: E) -> E {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1E(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: E = unsafe { sE };
// CHECK-NEXT:     let mut [[V_53:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_54:__v[0-9]+]]: u64 = ((([[V_53]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_54]]);
// CHECK-NEXT:     y = retmeE(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2E(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: E = unsafe { sE };
// CHECK-NEXT:     let mut [[V_55:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_56:__v[0-9]+]]: u64 = ((([[V_55]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_56]]);
// CHECK-NEXT:     let mut [[V_57:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_58:__v[0-9]+]]: u64 = (((([[V_57]] as u32) as i32) % (15 as i32)) as i64) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_58]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitE() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3E(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_59:__v[0-9]+]]: u64 = unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_60:__v[0-9]+]]: u64 = ((([[V_59]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sE.__slate_bits_0)).__set_k([[V_60]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitE()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testE() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: E = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sE) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) {
// CHECK-NEXT:         let mut [[V_61:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_62:__v[0-9]+]]: *mut i8 = unsafe { [[V_61]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_62]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_61]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_63:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_64:__v[0-9]+]]: i32 = [[V_63]] + (1 as i32);
// CHECK-NEXT:         i = [[V_64]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sE.l = 5.25f64 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sE.__slate_bits_0)).__set_k((-(1 as i32) as i64) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sE.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sE };
// CHECK-NEXT:     r = fn1E(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sE.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sE.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sE };
// CHECK-NEXT:     r = fn2E(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sE.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sE.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sE };
// CHECK-NEXT:     r = fn3E(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sE.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || x.l != unsafe { sE.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct F {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits5U0,
// CHECK-NEXT:     l: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sF: F = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeF(mut x: F) -> F {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1F(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: F = unsafe { sF };
// CHECK-NEXT:     let mut [[V_65:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_66:__v[0-9]+]]: u64 = ((([[V_65]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_66]]);
// CHECK-NEXT:     y = retmeF(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2F(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: F = unsafe { sF };
// CHECK-NEXT:     let mut [[V_67:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_68:__v[0-9]+]]: u64 = ((([[V_67]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_68]]);
// CHECK-NEXT:     let mut [[V_69:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_70:__v[0-9]+]]: u64 = (((([[V_69]] as u32) as i32) % (15 as i32)) as i64) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_70]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitF() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3F(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_71:__v[0-9]+]]: u64 = unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_72:__v[0-9]+]]: u64 = ((([[V_71]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sF.__slate_bits_0)).__set_k([[V_72]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitF()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testF() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: F = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sF) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) {
// CHECK-NEXT:         let mut [[V_73:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_74:__v[0-9]+]]: *mut i8 = unsafe { [[V_73]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_74]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_73]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_75:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_76:__v[0-9]+]]: i32 = [[V_75]] + (1 as i32);
// CHECK-NEXT:         i = [[V_76]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sF.l = 5.25f64 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sF.__slate_bits_0)).__set_k((-(1 as i32) as i64) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sF.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sF };
// CHECK-NEXT:     r = fn1F(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sF.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sF.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sF };
// CHECK-NEXT:     r = fn2F(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sF.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sF.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sF };
// CHECK-NEXT:     r = fn3F(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sF.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || x.l != unsafe { sF.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct G {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits6U0,
// CHECK-NEXT:     l: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sG: G = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeG(mut x: G) -> G {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1G(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: G = unsafe { sG };
// CHECK-NEXT:     let mut [[V_77:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_78:__v[0-9]+]]: u32 = (([[V_77]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_78]]);
// CHECK-NEXT:     y = retmeG(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2G(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: G = unsafe { sG };
// CHECK-NEXT:     let mut [[V_79:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_80:__v[0-9]+]]: u32 = (([[V_79]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_80]]);
// CHECK-NEXT:     let mut [[V_81:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_82:__v[0-9]+]]: u32 = (([[V_81]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_82]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitG() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3G(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_83:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_84:__v[0-9]+]]: u32 = (([[V_83]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sG.__slate_bits_0)).__set_k([[V_84]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitG()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testG() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: G = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sG) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) {
// CHECK-NEXT:         let mut [[V_85:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_86:__v[0-9]+]]: *mut i8 = unsafe { [[V_85]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_86]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_85]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_87:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_88:__v[0-9]+]]: i32 = [[V_87]] + (1 as i32);
// CHECK-NEXT:         i = [[V_88]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sG.l = 5.25f64 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sG.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sG.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sG };
// CHECK-NEXT:     r = fn1G(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sG.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sG.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sG };
// CHECK-NEXT:     r = fn2G(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sG.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sG.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sG };
// CHECK-NEXT:     r = fn3G(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sG.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sG.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct H {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits7U0,
// CHECK-NEXT:     l: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sH: H = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeH(mut x: H) -> H {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1H(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: H = unsafe { sH };
// CHECK-NEXT:     let mut [[V_89:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_90:__v[0-9]+]]: u32 = (([[V_89]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_90]]);
// CHECK-NEXT:     y = retmeH(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2H(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: H = unsafe { sH };
// CHECK-NEXT:     let mut [[V_91:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_92:__v[0-9]+]]: u32 = (([[V_91]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_92]]);
// CHECK-NEXT:     let mut [[V_93:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_94:__v[0-9]+]]: u32 = (([[V_93]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_94]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitH() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3H(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_95:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_96:__v[0-9]+]]: u32 = (([[V_95]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sH.__slate_bits_0)).__set_k([[V_96]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitH()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testH() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: H = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sH) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) {
// CHECK-NEXT:         let mut [[V_97:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_98:__v[0-9]+]]: *mut i8 = unsafe { [[V_97]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_98]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_97]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_99:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_100:__v[0-9]+]]: i32 = [[V_99]] + (1 as i32);
// CHECK-NEXT:         i = [[V_100]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sH.l = 5.25f64 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sH.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sH.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sH };
// CHECK-NEXT:     r = fn1H(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sH.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sH.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sH };
// CHECK-NEXT:     r = fn2H(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sH.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sH.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sH };
// CHECK-NEXT:     r = fn3H(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sH.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sH.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct I {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits8U0,
// CHECK-NEXT:     l: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sI: I = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeI(mut x: I) -> I {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1I(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: I = unsafe { sI };
// CHECK-NEXT:     let mut [[V_101:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_102:__v[0-9]+]]: u16 = ((([[V_101]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_102]]);
// CHECK-NEXT:     y = retmeI(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2I(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: I = unsafe { sI };
// CHECK-NEXT:     let mut [[V_103:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_104:__v[0-9]+]]: u16 = ((([[V_103]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_104]]);
// CHECK-NEXT:     let mut [[V_105:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_106:__v[0-9]+]]: u16 = (((([[V_105]] as u32) as i32) % (15 as i32)) as i16) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_106]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitI() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3I(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_107:__v[0-9]+]]: u16 = unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_108:__v[0-9]+]]: u16 = ((([[V_107]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sI.__slate_bits_0)).__set_k([[V_108]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitI()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testI() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: I = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sI) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) {
// CHECK-NEXT:         let mut [[V_109:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_110:__v[0-9]+]]: *mut i8 = unsafe { [[V_109]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_110]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_109]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_111:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_112:__v[0-9]+]]: i32 = [[V_111]] + (1 as i32);
// CHECK-NEXT:         i = [[V_112]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sI.l = 5.25f64 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sI.__slate_bits_0)).__set_k((-(1 as i32) as i16) as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sI.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sI };
// CHECK-NEXT:     r = fn1I(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sI.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sI.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sI };
// CHECK-NEXT:     r = fn2I(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sI.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sI.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sI };
// CHECK-NEXT:     r = fn3I(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sI.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || x.l != unsafe { sI.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct J {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits9U0,
// CHECK-NEXT:     l: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sJ: J = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeJ(mut x: J) -> J {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1J(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: J = unsafe { sJ };
// CHECK-NEXT:     let mut [[V_113:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_114:__v[0-9]+]]: u16 = ((([[V_113]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_114]]);
// CHECK-NEXT:     y = retmeJ(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2J(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: J = unsafe { sJ };
// CHECK-NEXT:     let mut [[V_115:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_116:__v[0-9]+]]: u16 = ((([[V_115]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_116]]);
// CHECK-NEXT:     let mut [[V_117:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_118:__v[0-9]+]]: u16 = (((([[V_117]] as u32) as i32) % (15 as i32)) as i16) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_118]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitJ() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3J(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_119:__v[0-9]+]]: u16 = unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_120:__v[0-9]+]]: u16 = ((([[V_119]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sJ.__slate_bits_0)).__set_k([[V_120]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitJ()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testJ() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: J = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sJ) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (4 as u64) {
// CHECK-NEXT:         let mut [[V_121:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_122:__v[0-9]+]]: *mut i8 = unsafe { [[V_121]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_122]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_121]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_123:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_124:__v[0-9]+]]: i32 = [[V_123]] + (1 as i32);
// CHECK-NEXT:         i = [[V_124]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sJ.l = 5.25f64 as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sJ.__slate_bits_0)).__set_k((-(1 as i32) as i16) as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sJ.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sJ };
// CHECK-NEXT:     r = fn1J(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || ((x.l as u32) as i32) != (((unsafe { sJ.l }) as u32) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sJ.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sJ };
// CHECK-NEXT:     r = fn2J(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || ((x.l as u32) as i32) != (((unsafe { sJ.l }) as u32) as i32)
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sJ.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sJ };
// CHECK-NEXT:     r = fn3J(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sJ.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || ((x.l as u32) as i32) != (((unsafe { sJ.l }) as u32) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct K {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits10U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sK: K = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeK(mut x: K) -> K {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1K(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: K = unsafe { sK };
// CHECK-NEXT:     let mut [[V_125:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_126:__v[0-9]+]]: u32 = (([[V_125]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_126]]);
// CHECK-NEXT:     y = retmeK(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2K(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: K = unsafe { sK };
// CHECK-NEXT:     let mut [[V_127:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_128:__v[0-9]+]]: u32 = (([[V_127]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_128]]);
// CHECK-NEXT:     let mut [[V_129:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_130:__v[0-9]+]]: u32 = (([[V_129]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_130]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitK() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3K(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_131:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_132:__v[0-9]+]]: u32 = (([[V_131]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sK.__slate_bits_0)).__set_k([[V_132]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitK()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testK() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: K = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sK) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (4 as u64) {
// CHECK-NEXT:         let mut [[V_133:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_134:__v[0-9]+]]: *mut i8 = unsafe { [[V_133]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_134]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_133]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_135:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_136:__v[0-9]+]]: i32 = [[V_135]] + (1 as i32);
// CHECK-NEXT:         i = [[V_136]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*std::ptr::addr_of_mut!(sK.__slate_bits_0)).__set_l(5.25f64 as u32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sK.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sK.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sK };
// CHECK-NEXT:     r = fn1K(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_l() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_l() }) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sK.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sK };
// CHECK-NEXT:     r = fn2K(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_l() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_l() }) as i32)
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sK.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sK };
// CHECK-NEXT:     r = fn3K(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || (x.__slate_bits_0.__get_l() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sK.__slate_bits_0)).__get_l() }) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct L {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits11U0,
// CHECK-NEXT:     l: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sL: L = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeL(mut x: L) -> L {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1L(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: L = unsafe { sL };
// CHECK-NEXT:     let mut [[V_137:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_138:__v[0-9]+]]: u32 = (([[V_137]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_138]]);
// CHECK-NEXT:     y = retmeL(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2L(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: L = unsafe { sL };
// CHECK-NEXT:     let mut [[V_139:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_140:__v[0-9]+]]: u32 = (([[V_139]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_140]]);
// CHECK-NEXT:     let mut [[V_141:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_142:__v[0-9]+]]: u32 = (([[V_141]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_142]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitL() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3L(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_143:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_144:__v[0-9]+]]: u32 = (([[V_143]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sL.__slate_bits_0)).__set_k([[V_144]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitL()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testL() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: L = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sL) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_11:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         let mut [[V_145:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_146:__v[0-9]+]]: *mut i8 = unsafe { [[V_145]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_146]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_145]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_147:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_148:__v[0-9]+]]: i32 = [[V_147]] + (1 as i32);
// CHECK-NEXT:         i = [[V_148]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sL.l = 5.25f64 as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sL.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sL.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sL };
// CHECK-NEXT:     r = fn1L(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sL.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sL.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sL };
// CHECK-NEXT:     r = fn2L(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sL.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sL.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sL };
// CHECK-NEXT:     r = fn3L(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sL.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sL.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct M {
// CHECK-NEXT:     l: u32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits12U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sM: M = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeM(mut x: M) -> M {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1M(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: M = unsafe { sM };
// CHECK-NEXT:     let mut [[V_149:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_150:__v[0-9]+]]: u32 = (([[V_149]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_150]]);
// CHECK-NEXT:     y = retmeM(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2M(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: M = unsafe { sM };
// CHECK-NEXT:     let mut [[V_151:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_152:__v[0-9]+]]: u32 = (([[V_151]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_152]]);
// CHECK-NEXT:     let mut [[V_153:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_154:__v[0-9]+]]: u32 = (([[V_153]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_154]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitM() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3M(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_155:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_156:__v[0-9]+]]: u32 = (([[V_155]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sM.__slate_bits_0)).__set_k([[V_156]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitM()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testM() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: M = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sM) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_12:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         let mut [[V_157:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_158:__v[0-9]+]]: *mut i8 = unsafe { [[V_157]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_158]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_157]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_159:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_160:__v[0-9]+]]: i32 = [[V_159]] + (1 as i32);
// CHECK-NEXT:         i = [[V_160]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sM.l = 5.25f64 as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sM.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sM.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sM };
// CHECK-NEXT:     r = fn1M(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sM.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sM.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sM };
// CHECK-NEXT:     r = fn2M(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sM.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sM.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sM };
// CHECK-NEXT:     r = fn3M(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sM.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sM.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct N {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits13U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sN: N = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeN(mut x: N) -> N {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1N(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: N = unsafe { sN };
// CHECK-NEXT:     let mut [[V_161:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_162:__v[0-9]+]]: u64 = ((([[V_161]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_162]]);
// CHECK-NEXT:     y = retmeN(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2N(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: N = unsafe { sN };
// CHECK-NEXT:     let mut [[V_163:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_164:__v[0-9]+]]: u64 = ((([[V_163]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_164]]);
// CHECK-NEXT:     let mut [[V_165:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_166:__v[0-9]+]]: u64 = (((([[V_165]] as u32) as i32) % (15 as i32)) as i64) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_166]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitN() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3N(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_167:__v[0-9]+]]: u64 = unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_168:__v[0-9]+]]: u64 = ((([[V_167]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sN.__slate_bits_0)).__set_k([[V_168]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitN()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testN() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: N = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sN) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_13:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         let mut [[V_169:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_170:__v[0-9]+]]: *mut i8 = unsafe { [[V_169]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_170]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_169]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_171:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_172:__v[0-9]+]]: i32 = [[V_171]] + (1 as i32);
// CHECK-NEXT:         i = [[V_172]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*std::ptr::addr_of_mut!(sN.__slate_bits_0)).__set_l(5.25f64 as u64);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sN.__slate_bits_0)).__set_k((-(1 as i32) as i64) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sN.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sN };
// CHECK-NEXT:     r = fn1N(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_l() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_l() }) as u32) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sN.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sN };
// CHECK-NEXT:     r = fn2N(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_l() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_l() }) as u32) as i32)
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sN.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sN };
// CHECK-NEXT:     r = fn3N(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_l() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sN.__slate_bits_0)).__get_l() }) as u32) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct O {
// CHECK-NEXT:     l: u64,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits14U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sO: O = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeO(mut x: O) -> O {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1O(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: O = unsafe { sO };
// CHECK-NEXT:     let mut [[V_173:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_174:__v[0-9]+]]: u64 = ((([[V_173]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_174]]);
// CHECK-NEXT:     y = retmeO(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2O(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: O = unsafe { sO };
// CHECK-NEXT:     let mut [[V_175:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_176:__v[0-9]+]]: u64 = ((([[V_175]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_176]]);
// CHECK-NEXT:     let mut [[V_177:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_178:__v[0-9]+]]: u64 = (((([[V_177]] as u32) as i32) % (15 as i32)) as i64) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_178]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitO() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3O(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_179:__v[0-9]+]]: u64 = unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_180:__v[0-9]+]]: u64 = ((([[V_179]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sO.__slate_bits_0)).__set_k([[V_180]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitO()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testO() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: O = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sO) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_14:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) {
// CHECK-NEXT:         let mut [[V_181:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_182:__v[0-9]+]]: *mut i8 = unsafe { [[V_181]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_182]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_181]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_183:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_184:__v[0-9]+]]: i32 = [[V_183]] + (1 as i32);
// CHECK-NEXT:         i = [[V_184]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sO.l = 5.25f64 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sO.__slate_bits_0)).__set_k((-(1 as i32) as i64) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sO.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sO };
// CHECK-NEXT:     r = fn1O(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sO.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sO.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sO };
// CHECK-NEXT:     r = fn2O(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sO.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sO.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sO };
// CHECK-NEXT:     r = fn3O(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sO.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || x.l != unsafe { sO.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct P {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits15U0,
// CHECK-NEXT:     l: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sP: P = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeP(mut x: P) -> P {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1P(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: P = unsafe { sP };
// CHECK-NEXT:     let mut [[V_185:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_186:__v[0-9]+]]: u64 = ((([[V_185]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_186]]);
// CHECK-NEXT:     y = retmeP(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2P(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: P = unsafe { sP };
// CHECK-NEXT:     let mut [[V_187:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_188:__v[0-9]+]]: u64 = ((([[V_187]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_188]]);
// CHECK-NEXT:     let mut [[V_189:__v[0-9]+]]: u64 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_190:__v[0-9]+]]: u64 = (((([[V_189]] as u32) as i32) % (15 as i32)) as i64) as u64;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_190]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitP() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3P(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_191:__v[0-9]+]]: u64 = unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_192:__v[0-9]+]]: u64 = ((([[V_191]] as u32) as i32) as u32).wrapping_add(x) as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sP.__slate_bits_0)).__set_k([[V_192]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitP()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testP() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: P = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sP) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_15:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) {
// CHECK-NEXT:         let mut [[V_193:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_194:__v[0-9]+]]: *mut i8 = unsafe { [[V_193]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_194]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_193]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_195:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_196:__v[0-9]+]]: i32 = [[V_195]] + (1 as i32);
// CHECK-NEXT:         i = [[V_196]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sP.l = 5.25f64 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sP.__slate_bits_0)).__set_k((-(1 as i32) as i64) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sP.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sP };
// CHECK-NEXT:     r = fn1P(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sP.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sP.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sP };
// CHECK-NEXT:     r = fn2P(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sP.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sP.__slate_bits_0)).__set_k(v as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sP };
// CHECK-NEXT:     r = fn3P(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sP.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || x.l != unsafe { sP.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Q {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits16U0,
// CHECK-NEXT:     l: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sQ: Q = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeQ(mut x: Q) -> Q {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1Q(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: Q = unsafe { sQ };
// CHECK-NEXT:     let mut [[V_197:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_198:__v[0-9]+]]: u32 = (([[V_197]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_198]]);
// CHECK-NEXT:     y = retmeQ(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2Q(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: Q = unsafe { sQ };
// CHECK-NEXT:     let mut [[V_199:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_200:__v[0-9]+]]: u32 = (([[V_199]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_200]]);
// CHECK-NEXT:     let mut [[V_201:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_202:__v[0-9]+]]: u32 = (([[V_201]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_202]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitQ() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3Q(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_203:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_204:__v[0-9]+]]: u32 = (([[V_203]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sQ.__slate_bits_0)).__set_k([[V_204]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitQ()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testQ() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: Q = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sQ) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_16:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) {
// CHECK-NEXT:         let mut [[V_205:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_206:__v[0-9]+]]: *mut i8 = unsafe { [[V_205]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_206]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_205]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_207:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_208:__v[0-9]+]]: i32 = [[V_207]] + (1 as i32);
// CHECK-NEXT:         i = [[V_208]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sQ.l = 5.25f64 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sQ.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sQ.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sQ };
// CHECK-NEXT:     r = fn1Q(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sQ.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sQ.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sQ };
// CHECK-NEXT:     r = fn2Q(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sQ.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sQ.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sQ };
// CHECK-NEXT:     r = fn3Q(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sQ.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sQ.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct R {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits17U0,
// CHECK-NEXT:     l: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sR: R = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeR(mut x: R) -> R {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1R(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: R = unsafe { sR };
// CHECK-NEXT:     let mut [[V_209:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_210:__v[0-9]+]]: u32 = (([[V_209]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_210]]);
// CHECK-NEXT:     y = retmeR(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2R(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: R = unsafe { sR };
// CHECK-NEXT:     let mut [[V_211:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_212:__v[0-9]+]]: u32 = (([[V_211]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_212]]);
// CHECK-NEXT:     let mut [[V_213:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_214:__v[0-9]+]]: u32 = (([[V_213]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_214]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitR() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3R(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_215:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_216:__v[0-9]+]]: u32 = (([[V_215]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sR.__slate_bits_0)).__set_k([[V_216]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitR()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testR() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: R = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sR) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_17:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) {
// CHECK-NEXT:         let mut [[V_217:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_218:__v[0-9]+]]: *mut i8 = unsafe { [[V_217]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_218]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_217]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_219:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_220:__v[0-9]+]]: i32 = [[V_219]] + (1 as i32);
// CHECK-NEXT:         i = [[V_220]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sR.l = 5.25f64 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sR.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sR.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sR };
// CHECK-NEXT:     r = fn1R(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sR.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sR.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sR };
// CHECK-NEXT:     r = fn2R(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sR.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sR.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sR };
// CHECK-NEXT:     r = fn3R(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sR.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sR.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits18U0,
// CHECK-NEXT:     l: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sS: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeS(mut x: S) -> S {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1S(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: S = unsafe { sS };
// CHECK-NEXT:     let mut [[V_221:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_222:__v[0-9]+]]: u16 = ((([[V_221]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_222]]);
// CHECK-NEXT:     y = retmeS(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2S(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: S = unsafe { sS };
// CHECK-NEXT:     let mut [[V_223:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_224:__v[0-9]+]]: u16 = ((([[V_223]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_224]]);
// CHECK-NEXT:     let mut [[V_225:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_226:__v[0-9]+]]: u16 = (((([[V_225]] as u32) as i32) % (15 as i32)) as i16) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_226]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitS() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3S(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_227:__v[0-9]+]]: u16 = unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_228:__v[0-9]+]]: u16 = ((([[V_227]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sS.__slate_bits_0)).__set_k([[V_228]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitS()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testS() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sS) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_18:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) {
// CHECK-NEXT:         let mut [[V_229:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_230:__v[0-9]+]]: *mut i8 = unsafe { [[V_229]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_230]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_229]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_231:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_232:__v[0-9]+]]: i32 = [[V_231]] + (1 as i32);
// CHECK-NEXT:         i = [[V_232]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sS.l = 5.25f64 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sS.__slate_bits_0)).__set_k((-(1 as i32) as i16) as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sS.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sS };
// CHECK-NEXT:     r = fn1S(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sS.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sS.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sS };
// CHECK-NEXT:     r = fn2S(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sS.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sS.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sS };
// CHECK-NEXT:     r = fn3S(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sS.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || x.l != unsafe { sS.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits19U0,
// CHECK-NEXT:     l: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sT: T = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeT(mut x: T) -> T {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1T(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: T = unsafe { sT };
// CHECK-NEXT:     let mut [[V_233:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_234:__v[0-9]+]]: u16 = ((([[V_233]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_234]]);
// CHECK-NEXT:     y = retmeT(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2T(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: T = unsafe { sT };
// CHECK-NEXT:     let mut [[V_235:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_236:__v[0-9]+]]: u16 = ((([[V_235]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_236]]);
// CHECK-NEXT:     let mut [[V_237:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_238:__v[0-9]+]]: u16 = (((([[V_237]] as u32) as i32) % (15 as i32)) as i16) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_238]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitT() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3T(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_239:__v[0-9]+]]: u16 = unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_240:__v[0-9]+]]: u16 = ((([[V_239]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sT.__slate_bits_0)).__set_k([[V_240]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitT()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testT() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: T = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sT) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_19:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (4 as u64) {
// CHECK-NEXT:         let mut [[V_241:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_242:__v[0-9]+]]: *mut i8 = unsafe { [[V_241]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_242]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_241]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_243:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_244:__v[0-9]+]]: i32 = [[V_243]] + (1 as i32);
// CHECK-NEXT:         i = [[V_244]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sT.l = 5.25f64 as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sT.__slate_bits_0)).__set_k((-(1 as i32) as i16) as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sT.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sT };
// CHECK-NEXT:     r = fn1T(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || ((x.l as u32) as i32) != (((unsafe { sT.l }) as u32) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sT.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sT };
// CHECK-NEXT:     r = fn2T(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || ((x.l as u32) as i32) != (((unsafe { sT.l }) as u32) as i32)
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sT.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sT };
// CHECK-NEXT:     r = fn3T(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sT.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || ((x.l as u32) as i32) != (((unsafe { sT.l }) as u32) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct U {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits20U0,
// CHECK-NEXT:     l: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sU: U = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeU(mut x: U) -> U {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1U(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: U = unsafe { sU };
// CHECK-NEXT:     let mut [[V_245:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_246:__v[0-9]+]]: u16 = ((([[V_245]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_246]]);
// CHECK-NEXT:     y = retmeU(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2U(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: U = unsafe { sU };
// CHECK-NEXT:     let mut [[V_247:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_248:__v[0-9]+]]: u16 = ((([[V_247]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_248]]);
// CHECK-NEXT:     let mut [[V_249:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_250:__v[0-9]+]]: u16 = (((([[V_249]] as u32) as i32) % (15 as i32)) as i16) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_250]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitU() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3U(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_251:__v[0-9]+]]: u16 = unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_252:__v[0-9]+]]: u16 = ((([[V_251]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sU.__slate_bits_0)).__set_k([[V_252]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitU()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testU() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sU) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_20:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) {
// CHECK-NEXT:         let mut [[V_253:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_254:__v[0-9]+]]: *mut i8 = unsafe { [[V_253]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_254]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_253]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_255:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_256:__v[0-9]+]]: i32 = [[V_255]] + (1 as i32);
// CHECK-NEXT:         i = [[V_256]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sU.l = 5.25f64 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sU.__slate_bits_0)).__set_k((-(1 as i32) as i16) as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sU.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sU };
// CHECK-NEXT:     r = fn1U(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sU.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sU.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sU };
// CHECK-NEXT:     r = fn2U(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || x.l != unsafe { sU.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sU.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sU };
// CHECK-NEXT:     r = fn3U(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sU.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || x.l != unsafe { sU.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct V {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits21U0,
// CHECK-NEXT:     l: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sV: V = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeV(mut x: V) -> V {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1V(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: V = unsafe { sV };
// CHECK-NEXT:     let mut [[V_257:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_258:__v[0-9]+]]: u16 = ((([[V_257]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_258]]);
// CHECK-NEXT:     y = retmeV(y);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2V(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: V = unsafe { sV };
// CHECK-NEXT:     let mut [[V_259:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_260:__v[0-9]+]]: u16 = ((([[V_259]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_260]]);
// CHECK-NEXT:     let mut [[V_261:__v[0-9]+]]: u16 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_262:__v[0-9]+]]: u16 = (((([[V_261]] as u32) as i32) % (15 as i32)) as i16) as u16;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_262]]);
// CHECK-NEXT:     ((y.__slate_bits_0.__get_k() as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitV() -> u32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_k() }) as u32) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3V(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_263:__v[0-9]+]]: u16 = unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_264:__v[0-9]+]]: u16 = ((([[V_263]] as u32) as i32) as u32).wrapping_add(x) as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sV.__slate_bits_0)).__set_k([[V_264]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitV()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testV() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: V = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sV) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_21:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (4 as u64) {
// CHECK-NEXT:         let mut [[V_265:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_266:__v[0-9]+]]: *mut i8 = unsafe { [[V_265]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_266]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_265]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_267:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_268:__v[0-9]+]]: i32 = [[V_267]] + (1 as i32);
// CHECK-NEXT:         i = [[V_268]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sV.l = 5.25f64 as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sV.__slate_bits_0)).__set_k((-(1 as i32) as i16) as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = (((unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_k() }) as u32) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sV.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sV };
// CHECK-NEXT:     r = fn1V(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || ((x.l as u32) as i32) != (((unsafe { sV.l }) as u32) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sV.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sV };
// CHECK-NEXT:     r = fn2V(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_k() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:         || ((x.l as u32) as i32) != (((unsafe { sV.l }) as u32) as i32)
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sV.__slate_bits_0)).__set_k(v as u16);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sV };
// CHECK-NEXT:     r = fn3V(a);
// CHECK-NEXT:     if ((x.__slate_bits_0.__get_i() as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_i() }) as u32) as i32)
// CHECK-NEXT:         || ((x.__slate_bits_0.__get_j() as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_j() }) as u32) as i32)
// CHECK-NEXT:         || ((((unsafe { (*std::ptr::addr_of!(sV.__slate_bits_0)).__get_k() }) as u32) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             != r
// CHECK-NEXT:         || ((x.l as u32) as i32) != (((unsafe { sV.l }) as u32) as i32)
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct W {
// CHECK-NEXT:     l: LongDouble,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits22U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sW: W = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeW(mut x: W) -> W {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1W(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: W = unsafe { sW };
// CHECK-NEXT:     let mut [[V_269:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_270:__v[0-9]+]]: u32 = (([[V_269]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_270]]);
// CHECK-NEXT:     y = retmeW(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2W(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: W = unsafe { sW };
// CHECK-NEXT:     let mut [[V_271:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_272:__v[0-9]+]]: u32 = (([[V_271]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_272]]);
// CHECK-NEXT:     let mut [[V_273:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_274:__v[0-9]+]]: u32 = (([[V_273]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_274]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitW() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3W(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_275:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_276:__v[0-9]+]]: u32 = (([[V_275]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sW.__slate_bits_0)).__set_k([[V_276]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitW()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testW() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: W = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sW) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_22:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (32 as u64) {
// CHECK-NEXT:         let mut [[V_277:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_278:__v[0-9]+]]: *mut i8 = unsafe { [[V_277]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_278]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_277]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_279:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_280:__v[0-9]+]]: i32 = [[V_279]] + (1 as i32);
// CHECK-NEXT:         i = [[V_280]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (8 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sW.l = __slate_f80_from_f64(5.25f64);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sW.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sW.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sW };
// CHECK-NEXT:     r = fn1W(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sW.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sW.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sW };
// CHECK-NEXT:     r = fn2W(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sW.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sW.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sW };
// CHECK-NEXT:     r = fn3W(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sW.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sW.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct X {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits23U0,
// CHECK-NEXT:     l: LongDouble,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sX: X = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeX(mut x: X) -> X {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1X(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: X = unsafe { sX };
// CHECK-NEXT:     let mut [[V_281:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_282:__v[0-9]+]]: u32 = (([[V_281]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_282]]);
// CHECK-NEXT:     y = retmeX(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2X(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: X = unsafe { sX };
// CHECK-NEXT:     let mut [[V_283:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_284:__v[0-9]+]]: u32 = (([[V_283]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_284]]);
// CHECK-NEXT:     let mut [[V_285:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_286:__v[0-9]+]]: u32 = (([[V_285]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_286]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitX() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3X(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_287:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_288:__v[0-9]+]]: u32 = (([[V_287]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sX.__slate_bits_0)).__set_k([[V_288]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitX()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testX() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: X = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sX) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_23:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (32 as u64) {
// CHECK-NEXT:         let mut [[V_289:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_290:__v[0-9]+]]: *mut i8 = unsafe { [[V_289]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_290]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_289]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_291:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_292:__v[0-9]+]]: i32 = [[V_291]] + (1 as i32);
// CHECK-NEXT:         i = [[V_292]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (8 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sX.l = __slate_f80_from_f64(5.25f64);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sX.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sX.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sX };
// CHECK-NEXT:     r = fn1X(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sX.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sX.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sX };
// CHECK-NEXT:     r = fn2X(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sX.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sX.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sX };
// CHECK-NEXT:     r = fn3X(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sX.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sX.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Y {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits24U0,
// CHECK-NEXT:     l: LongDouble,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sY: Y = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeY(mut x: Y) -> Y {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1Y(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: Y = unsafe { sY };
// CHECK-NEXT:     let mut [[V_293:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_294:__v[0-9]+]]: u32 = (([[V_293]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_294]]);
// CHECK-NEXT:     y = retmeY(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2Y(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: Y = unsafe { sY };
// CHECK-NEXT:     let mut [[V_295:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_296:__v[0-9]+]]: u32 = (([[V_295]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_296]]);
// CHECK-NEXT:     let mut [[V_297:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_298:__v[0-9]+]]: u32 = (([[V_297]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_298]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitY() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3Y(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_299:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_300:__v[0-9]+]]: u32 = (([[V_299]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sY.__slate_bits_0)).__set_k([[V_300]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitY()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testY() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: Y = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sY) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_24:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (32 as u64) {
// CHECK-NEXT:         let mut [[V_301:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_302:__v[0-9]+]]: *mut i8 = unsafe { [[V_301]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_302]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_301]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_303:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_304:__v[0-9]+]]: i32 = [[V_303]] + (1 as i32);
// CHECK-NEXT:         i = [[V_304]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (8 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sY.l = __slate_f80_from_f64(5.25f64);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sY.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sY.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sY };
// CHECK-NEXT:     r = fn1Y(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sY.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sY.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sY };
// CHECK-NEXT:     r = fn2Y(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sY.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sY.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sY };
// CHECK-NEXT:     r = fn3Y(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sY.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sY.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Z {
// CHECK-NEXT:     l: LongDouble,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits25U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sZ: Z = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn retmeZ(mut x: Z) -> Z {
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1Z(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: Z = unsafe { sZ };
// CHECK-NEXT:     let mut [[V_305:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_306:__v[0-9]+]]: u32 = (([[V_305]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_306]]);
// CHECK-NEXT:     y = retmeZ(y);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2Z(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: Z = unsafe { sZ };
// CHECK-NEXT:     let mut [[V_307:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_308:__v[0-9]+]]: u32 = (([[V_307]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_308]]);
// CHECK-NEXT:     let mut [[V_309:__v[0-9]+]]: u32 = y.__slate_bits_0.__get_k();
// CHECK-NEXT:     let mut [[V_310:__v[0-9]+]]: u32 = (([[V_309]] as i32) % (15 as i32)) as u32;
// CHECK-NEXT:     y.__slate_bits_0.__set_k([[V_310]]);
// CHECK-NEXT:     (y.__slate_bits_0.__get_k() as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retitZ() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3Z(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_311:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_312:__v[0-9]+]]: u32 = (([[V_311]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sZ.__slate_bits_0)).__set_k([[V_312]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     retitZ()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testZ() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut v: u32 = 0 as u32;
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: Z = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i8 = (unsafe { std::ptr::addr_of_mut!(sZ) }) as *mut i8;
// CHECK-NEXT:     '[[SLATE_BREAK_25:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (32 as u64) {
// CHECK-NEXT:         let mut [[V_313:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_314:__v[0-9]+]]: *mut i8 = unsafe { [[V_313]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_314]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_313]] = (myrnd() as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_315:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_316:__v[0-9]+]]: i32 = [[V_315]] + (1 as i32);
// CHECK-NEXT:         i = [[V_316]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (8 as i32) == (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             sZ.l = __slate_f80_from_f64(5.25f64);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sZ.__slate_bits_0)).__set_k(-(1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     mask = ((unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_k() }) as i32) as u32;
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sZ.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sZ };
// CHECK-NEXT:     r = fn1Z(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sZ.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sZ.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sZ };
// CHECK-NEXT:     r = fn2Z(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_k() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_k() }) as i32)
// CHECK-NEXT:         || x.l != unsafe { sZ.l }
// CHECK-NEXT:         || (v.wrapping_add(a) & mask) % ((15 as i32) as u32) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     v = myrnd();
// CHECK-NEXT:     a = myrnd();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sZ.__slate_bits_0)).__set_k(v);
// CHECK-NEXT:     }
// CHECK-NEXT:     x = unsafe { sZ };
// CHECK-NEXT:     r = fn3Z(a);
// CHECK-NEXT:     if (x.__slate_bits_0.__get_i() as i32)
// CHECK-NEXT:         != ((unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_i() }) as i32)
// CHECK-NEXT:         || (x.__slate_bits_0.__get_j() as i32)
// CHECK-NEXT:             != ((unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_j() }) as i32)
// CHECK-NEXT:         || (((unsafe { (*std::ptr::addr_of!(sZ.__slate_bits_0)).__get_k() }) as i32) as u32) != r
// CHECK-NEXT:         || x.l != unsafe { sZ.l }
// CHECK-NEXT:         || v.wrapping_add(a) & mask != r
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     testA();
// CHECK-NEXT:     testB();
// CHECK-NEXT:     testC();
// CHECK-NEXT:     testD();
// CHECK-NEXT:     testE();
// CHECK-NEXT:     testF();
// CHECK-NEXT:     testG();
// CHECK-NEXT:     testH();
// CHECK-NEXT:     testI();
// CHECK-NEXT:     testJ();
// CHECK-NEXT:     testK();
// CHECK-NEXT:     testL();
// CHECK-NEXT:     testM();
// CHECK-NEXT:     testN();
// CHECK-NEXT:     testO();
// CHECK-NEXT:     testP();
// CHECK-NEXT:     testQ();
// CHECK-NEXT:     testR();
// CHECK-NEXT:     testS();
// CHECK-NEXT:     testT();
// CHECK-NEXT:     testU();
// CHECK-NEXT:     testV();
// CHECK-NEXT:     testW();
// CHECK-NEXT:     testX();
// CHECK-NEXT:     testY();
// CHECK-NEXT:     testZ();
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
