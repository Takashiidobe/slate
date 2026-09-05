/*
 * long_double link fixture: native helper side.
 *
 * Compiled to an object file (link.o) and linked into BOTH the C reference
 * binary and the translated Rust binary.  Do not translate this file.
 */
#include <float.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_MSC_VER)
#define NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define ALIGNOF(T) _Alignof(T)
#else
#define ALIGNOF(T)                                                             \
  offsetof(                                                                    \
      struct {                                                                 \
        char c;                                                                \
        T    x;                                                                \
      },                                                                       \
      x)
#endif

struct ld_box {
  unsigned char tag;
  long double   x;
  uint32_t      tail;
};

struct ld_pair {
  long double a;
  long double b;
};

struct ld_nested {
  uint16_t       head;
  struct ld_pair pair;
  unsigned char  bytes[3];
  long double    z;
};

union ld_union {
  long double   ld;
  unsigned char bytes[sizeof(long double)];
  uint64_t      u64;
};

typedef long double (*ld_binary_fn)(long double, long double);
typedef long double (*ld_callback_fn)(long double, long double);
long double         ext_global_ld = 0x1.23456789abcdefp+10L;

NOINLINE long double ext_identity(long double x) { return x; }
NOINLINE long double ext_add(long double a, long double b) { return a + b; }
NOINLINE long double ext_sub(long double a, long double b) { return a - b; }
NOINLINE long double ext_mul(long double a, long double b) { return a * b; }
NOINLINE long double ext_div(long double a, long double b) { return a / b; }
NOINLINE long double ext_neg(long double a) { return -a; }

NOINLINE int ext_eq(long double a, long double b) { return a == b; }
NOINLINE int ext_lt(long double a, long double b) { return a < b; }
NOINLINE int ext_le(long double a, long double b) { return a <= b; }

NOINLINE long double ext_from_i64(int64_t x) { return (long double)x; }
NOINLINE long double ext_from_u64(uint64_t x) { return (long double)x; }
NOINLINE int64_t     ext_to_i64(long double x) { return (int64_t)x; }
NOINLINE uint64_t    ext_to_u64(long double x) { return (uint64_t)x; }
NOINLINE long double ext_from_double(double x) { return (long double)x; }
NOINLINE double      ext_to_double(long double x) { return (double)x; }
NOINLINE long double ext_from_float(float x) { return (long double)x; }
NOINLINE float       ext_to_float(long double x) { return (float)x; }

NOINLINE long double ext_sum10(long double a0, long double a1, long double a2,
                               long double a3, long double a4, long double a5,
                               long double a6, long double a7, long double a8,
                               long double a9) {
  return (((((a0 + a1) + a2) + a3) + a4) + ((((a5 + a6) + a7) + a8) + a9));
}

/* Mix integer, double, float and long-double arguments so each ABI has to
 * classify a nontrivial call correctly.  Volatile temporaries prevent this
 * helper from becoming an accidental constant-expression oracle. */
NOINLINE long double ext_mix_abi(int64_t i0, double d0, long double a,
                                 uint32_t u0, long double b, float f0,
                                 long double c, int i1, double d1,
                                 long double d) {
  volatile long double r = a;
  r                      = r + b;
  r                      = r - c;
  r                      = r + d;
  r                      = r + (long double)i0;
  r                      = r - (long double)u0;
  r                      = r + (long double)d0;
  r                      = r - (long double)f0;
  r                      = r + (long double)i1;
  r                      = r - (long double)d1;
  return r;
}

NOINLINE struct ld_box ext_box_roundtrip(struct ld_box b) {
  b.x     = b.x + 0x1p-9L;
  b.tag   = (unsigned char)(b.tag ^ 0x5aU);
  b.tail ^= UINT32_C(0xa5a55a5a);
  return b;
}

NOINLINE struct ld_pair ext_pair_make(long double a, long double b) {
  struct ld_pair p;
  p.a = a;
  p.b = b;
  return p;
}

NOINLINE struct ld_nested ext_nested_roundtrip(struct ld_nested n) {
  n.pair.a   += 1.0L;
  n.pair.b   -= 2.0L;
  n.z        *= 0.5L;
  n.head     ^= UINT16_C(0x55aa);
  n.bytes[0] ^= 1U;
  n.bytes[1] ^= 2U;
  n.bytes[2] ^= 4U;
  return n;
}

NOINLINE long double ext_array_sum(const long double *p, size_t n) {
  long double r = 0.0L;
  size_t      i;
  for (i = 0; i < n; ++i)
    r += p[i];
  return r;
}

NOINLINE void        ext_store(long double *p, long double x) { *p = x; }
NOINLINE long double ext_load(const long double *p) { return *p; }

NOINLINE long double ext_call_cb(ld_callback_fn cb, long double a,
                                 long double b) {
  return cb(a, b);
}

NOINLINE long double ext_vsum(int n, ...) {
  va_list     ap;
  long double r = 0.0L;
  int         i;
  va_start(ap, n);
  for (i = 0; i < n; ++i)
    r += va_arg(ap, long double);
  va_end(ap);
  return r;
}

NOINLINE void        ext_global_set(long double x) { ext_global_ld = x; }
NOINLINE long double ext_global_get(void) { return ext_global_ld; }

NOINLINE size_t ext_sizeof_ld(void) { return sizeof(long double); }
NOINLINE size_t ext_alignof_ld(void) { return ALIGNOF(long double); }
NOINLINE size_t ext_sizeof_box(void) { return sizeof(struct ld_box); }
NOINLINE size_t ext_alignof_box(void) { return ALIGNOF(struct ld_box); }
NOINLINE size_t ext_offset_box_x(void) { return offsetof(struct ld_box, x); }
NOINLINE size_t ext_offset_box_tail(void) {
  return offsetof(struct ld_box, tail);
}
NOINLINE size_t ext_sizeof_pair(void) { return sizeof(struct ld_pair); }
NOINLINE size_t ext_alignof_pair(void) { return ALIGNOF(struct ld_pair); }
NOINLINE size_t ext_sizeof_nested(void) { return sizeof(struct ld_nested); }
NOINLINE size_t ext_alignof_nested(void) { return ALIGNOF(struct ld_nested); }
NOINLINE size_t ext_offset_nested_pair(void) {
  return offsetof(struct ld_nested, pair);
}
NOINLINE size_t ext_offset_nested_z(void) {
  return offsetof(struct ld_nested, z);
}
NOINLINE size_t ext_sizeof_union(void) { return sizeof(union ld_union); }
NOINLINE size_t ext_alignof_union(void) { return ALIGNOF(union ld_union); }

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(f128)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C, align(16))]
// LOWERING-AARCH64-GNU-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-X86_64-GNU-NEXT:         } else {
// LOWERING-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-X86_64-GNU-NEXT:             } else {
// LOWERING-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// LOWERING-X86_64-GNU-NEXT:                 } else {
// LOWERING-X86_64-GNU-NEXT:                     None
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-AARCH64-GNU-NEXT: struct __va_list {
// LOWERING-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct ld_box {
// LOWERING-NEXT:     tag: u8,
// LOWERING-X86_64-GNU-NEXT:     x: LongDouble,
// LOWERING-AARCH64-GNU-NEXT:     x: f128,
// LOWERING-NEXT:     tail: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct ld_nested {
// LOWERING-NEXT:     head: u16,
// LOWERING-NEXT:     pair: ld_pair,
// LOWERING-NEXT:     bytes: [u8; 3],
// LOWERING-X86_64-GNU-NEXT:     z: LongDouble,
// LOWERING-AARCH64-GNU-NEXT:     z: f128,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct ld_pair {
// LOWERING-X86_64-GNU-NEXT:     a: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     b: LongDouble,
// LOWERING-AARCH64-GNU-NEXT:     a: f128,
// LOWERING-AARCH64-GNU-NEXT:     b: f128,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union ld_union {
// LOWERING-X86_64-GNU-NEXT:     ld: LongDouble,
// LOWERING-AARCH64-GNU-NEXT:     ld: f128,
// LOWERING-NEXT:     bytes: [u8; 16],
// LOWERING-NEXT:     u64: u64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: static mut ext_global_ld: LongDouble = LongDouble([128, 247, 230, 213, 196, 179, 162, 145, 9, 64]);
// LOWERING-AARCH64-GNU-NEXT: static mut ext_global_ld: f128 = 1165.0844444444444292230400606058538f128;
// LOWERING-EMPTY:
// LOWERING-NEXT: struct __SlateVaArg {
// LOWERING-NEXT:     value: Box<dyn std::any::Any>,
// LOWERING-NEXT:     size: usize,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl __SlateVaArg {
// LOWERING-NEXT:     fn new<T: 'static>(value: T) -> Self {
// LOWERING-NEXT:         Self {
// LOWERING-NEXT:             value: Box::new(value),
// LOWERING-NEXT:             size: std::mem::size_of::<T>(),
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// LOWERING-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// LOWERING-NEXT:             return *value;
// LOWERING-NEXT:         }
// LOWERING-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             std::ptr::read_unaligned(
// LOWERING-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[derive(Clone)]
// LOWERING-NEXT: struct __SlateVaArgs {
// LOWERING-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// LOWERING-NEXT:     index: usize,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl __SlateVaArgs {
// LOWERING-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// LOWERING-NEXT:         Self {
// LOWERING-NEXT:             args: Some(std::rc::Rc::new(args)),
// LOWERING-NEXT:             index: 0,
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     const fn empty() -> Self {
// LOWERING-NEXT:         Self {
// LOWERING-NEXT:             args: None,
// LOWERING-NEXT:             index: 0,
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// LOWERING-NEXT:         let index = self.index;
// LOWERING-NEXT:         self.index += 1;
// LOWERING-NEXT:         if std::mem::size_of::<T>() == 0 {
// LOWERING-NEXT:             return unsafe { std::mem::zeroed() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// LOWERING-NEXT:         args[index].read::<T>()
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_identity({{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-AARCH64-GNU-NEXT: fn ext_identity({{arg[0-9]+}}: f128) -> f128 {
// LOWERING-NEXT:     return {{arg[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_add({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: fn ext_add({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_sub({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} - {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: fn ext_sub({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} - {{arg[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_mul({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: fn ext_mul({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_div({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} / {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: fn ext_div({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} / {{arg[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_neg({{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = -{{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: fn ext_neg({{arg[0-9]+}}: f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = -{{arg[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_eq({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn ext_eq({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} == {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_lt({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn ext_lt({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} < {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_le({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn ext_le({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} <= {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_from_i64({{arg[0-9]+}}: i64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i64({{arg[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT: fn ext_from_i64({{arg[0-9]+}}: i64) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_from_u64({{arg[0-9]+}}: u64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u64({{arg[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT: fn ext_from_u64({{arg[0-9]+}}: u64) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_to_i64({{arg[0-9]+}}: LongDouble) -> i64 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = __slate_f80_to_i64({{arg[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT: fn ext_to_i64({{arg[0-9]+}}: f128) -> i64 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = {{arg[0-9]+}} as i64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_to_u64({{arg[0-9]+}}: LongDouble) -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = __slate_f80_to_u64({{arg[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT: fn ext_to_u64({{arg[0-9]+}}: f128) -> u64 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_from_double({{arg[0-9]+}}: f64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT: fn ext_from_double({{arg[0-9]+}}: f64) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_to_double({{arg[0-9]+}}: LongDouble) -> f64 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{arg[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT: fn ext_to_double({{arg[0-9]+}}: f128) -> f64 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = {{arg[0-9]+}} as f64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_from_float({{arg[0-9]+}}: f32) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{arg[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT: fn ext_from_float({{arg[0-9]+}}: f32) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_to_float({{arg[0-9]+}}: LongDouble) -> f32 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = __slate_f80_to_f32({{arg[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT: fn ext_to_float({{arg[0-9]+}}: f128) -> f32 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{arg[0-9]+}} as f32;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_sum10(
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT: ) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT: ) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: /// Mix integer, double, float and long-double arguments so each ABI has to
// LOWERING-NEXT: /// classify a nontrivial call correctly.  Volatile temporaries prevent this
// LOWERING-NEXT: /// helper from becoming an accidental constant-expression oracle.
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_mix_abi(
// LOWERING-NEXT:     {{arg[0-9]+}}: i64,
// LOWERING-NEXT:     {{arg[0-9]+}}: f64,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-NEXT:     {{arg[0-9]+}}: u32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-NEXT:     {{arg[0-9]+}}: f32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: f64,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT: ) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT: ) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{arg[0-9]+}};
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_box_roundtrip({{arg[0-9]+}}: ld_box) -> ld_box {
// LOWERING-NEXT:     let mut b: ld_box = ld_box {
// LOWERING-NEXT:         tag: 0,
// LOWERING-X86_64-GNU-NEXT:         x: LongDouble([0; 10]),
// LOWERING-AARCH64-GNU-NEXT:         x: 0.0f128,
// LOWERING-NEXT:         tail: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = b.x;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 246, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = b.x;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.001953125f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     b.x = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = b.tag;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 90;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-NEXT:     b.tag = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 2779077210u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = b.tail;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// LOWERING-NEXT:     b.tail = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: ld_box = b;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_pair_make({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> ld_pair {
// LOWERING-AARCH64-GNU-NEXT: fn ext_pair_make({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> ld_pair {
// LOWERING-NEXT:     let mut __retval: ld_pair = ld_pair {
// LOWERING-X86_64-GNU-NEXT:         a: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         b: LongDouble([0; 10]),
// LOWERING-AARCH64-GNU-NEXT:         a: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:         b: 0.0f128,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     __retval.a = {{arg[0-9]+}};
// LOWERING-NEXT:     __retval.b = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: ld_pair = __retval;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_nested_roundtrip({{arg[0-9]+}}: ld_nested) -> ld_nested {
// LOWERING-NEXT:     let mut n: ld_nested = ld_nested {
// LOWERING-NEXT:         head: 0,
// LOWERING-NEXT:         pair: ld_pair {
// LOWERING-X86_64-GNU-NEXT:             a: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:             b: LongDouble([0; 10]),
// LOWERING-AARCH64-GNU-NEXT:             a: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:             b: 0.0f128,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         bytes: [0; 3],
// LOWERING-X86_64-GNU-NEXT:         z: LongDouble([0; 10]),
// LOWERING-AARCH64-GNU-NEXT:         z: 0.0f128,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     n = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = n.pair.a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = n.pair.a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     n.pair.a = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = n.pair.b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 2.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = n.pair.b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-NEXT:     n.pair.b = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = n.z;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e-01f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = n.z;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     n.z = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 21930;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = n.head;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = {{__v[0-9]+}} as u16;
// LOWERING-NEXT:     n.head = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = n.bytes[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = n.bytes[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = n.bytes[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: ld_nested = n;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_array_sum({{arg[0-9]+}}: *mut LongDouble, {{arg[0-9]+}}: u64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let mut p: *mut LongDouble = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT: unsafe fn ext_array_sum({{arg[0-9]+}}: *mut f128, {{arg[0-9]+}}: u64) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let mut p: *mut f128 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut n: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// LOWERING-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// LOWERING-NEXT:     let mut i: u64 = 0;
// LOWERING-NEXT:     p = {{arg[0-9]+}};
// LOWERING-NEXT:     n = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-NEXT:     r = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         i = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = n;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut LongDouble = p;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut LongDouble = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut f128 = p;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut f128 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = r;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:             r = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = r;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = r;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_store({{arg[0-9]+}}: *mut LongDouble, {{arg[0-9]+}}: LongDouble) {
// LOWERING-AARCH64-GNU-NEXT: unsafe fn ext_store({{arg[0-9]+}}: *mut f128, {{arg[0-9]+}}: f128) {
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{arg[0-9]+}} = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_load({{arg[0-9]+}}: *mut LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { *{{arg[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT: unsafe fn ext_load({{arg[0-9]+}}: *mut f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_call_cb(
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT: ) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(f128, f128) -> f128>,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT: ) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_vsum({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> LongDouble {
// LOWERING-AARCH64-GNU-NEXT: unsafe fn ext_vsum({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> f128 {
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// LOWERING-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// LOWERING-NEXT:     let mut i: i32 = 0;
// LOWERING-NEXT:     n = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-NEXT:     r = {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         ap = __slate_va_args.clone();
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = n;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = unsafe { ap.next_arg::<LongDouble>() };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = unsafe { ap.next_arg::<f128>() };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = r;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:             r = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = r;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = r;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_global_set({{arg[0-9]+}}: LongDouble) {
// LOWERING-AARCH64-GNU-NEXT: fn ext_global_set({{arg[0-9]+}}: f128) {
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         ext_global_ld = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_global_get() -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { ext_global_ld };
// LOWERING-AARCH64-GNU-NEXT: fn ext_global_get() -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { ext_global_ld };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_sizeof_ld() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_alignof_ld() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_sizeof_box() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<ld_box>() as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_alignof_box() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<ld_box>() as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_offset_box_x() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(ld_box, x) as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_offset_box_tail() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(ld_box, tail) as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_sizeof_pair() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<ld_pair>() as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_alignof_pair() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<ld_pair>() as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_sizeof_nested() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<ld_nested>() as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_alignof_nested() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<ld_nested>() as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_offset_nested_pair() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(ld_nested, pair) as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_offset_nested_z() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(ld_nested, z) as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_sizeof_union() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<ld_union>() as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn ext_alignof_union() -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<ld_union>() as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(f128)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C, align(16))]
// REWRITES-AARCH64-GNU-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-X86_64-GNU-NEXT:                 } else {
// REWRITES-X86_64-GNU-NEXT:                     None
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-AARCH64-GNU-NEXT: struct __va_list {
// REWRITES-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct ld_box {
// REWRITES-NEXT:     tag: u8,
// REWRITES-X86_64-GNU-NEXT:     x: LongDouble,
// REWRITES-AARCH64-GNU-NEXT:     x: f128,
// REWRITES-NEXT:     tail: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct ld_nested {
// REWRITES-NEXT:     head: u16,
// REWRITES-NEXT:     pair: ld_pair,
// REWRITES-NEXT:     bytes: [u8; 3],
// REWRITES-X86_64-GNU-NEXT:     z: LongDouble,
// REWRITES-AARCH64-GNU-NEXT:     z: f128,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct ld_pair {
// REWRITES-X86_64-GNU-NEXT:     a: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     b: LongDouble,
// REWRITES-AARCH64-GNU-NEXT:     a: f128,
// REWRITES-AARCH64-GNU-NEXT:     b: f128,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union ld_union {
// REWRITES-X86_64-GNU-NEXT:     ld: LongDouble,
// REWRITES-AARCH64-GNU-NEXT:     ld: f128,
// REWRITES-NEXT:     bytes: [u8; 16],
// REWRITES-NEXT:     u64: u64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-X86_64-GNU-NEXT: static mut ext_global_ld: LongDouble = LongDouble([128, 247, 230, 213, 196, 179, 162, 145, 9, 64]);
// REWRITES-AARCH64-GNU-NEXT: static mut ext_global_ld: f128 = 1165.0844444444444292230400606058538f128;
// REWRITES-EMPTY:
// REWRITES-NEXT: struct __SlateVaArg {
// REWRITES-NEXT:     value: Box<dyn std::any::Any>,
// REWRITES-NEXT:     size: usize,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl __SlateVaArg {
// REWRITES-NEXT:     fn new<T: 'static>(value: T) -> Self {
// REWRITES-NEXT:         Self {
// REWRITES-NEXT:             value: Box::new(value),
// REWRITES-NEXT:             size: std::mem::size_of::<T>(),
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// REWRITES-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// REWRITES-NEXT:             return *value;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             std::ptr::read_unaligned(
// REWRITES-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[derive(Clone)]
// REWRITES-NEXT: struct __SlateVaArgs {
// REWRITES-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// REWRITES-NEXT:     index: usize,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl __SlateVaArgs {
// REWRITES-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// REWRITES-NEXT:         Self {
// REWRITES-NEXT:             args: Some(std::rc::Rc::new(args)),
// REWRITES-NEXT:             index: 0,
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     const fn empty() -> Self {
// REWRITES-NEXT:         Self {
// REWRITES-NEXT:             args: None,
// REWRITES-NEXT:             index: 0,
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// REWRITES-NEXT:         let index = self.index;
// REWRITES-NEXT:         self.index += 1;
// REWRITES-NEXT:         if std::mem::size_of::<T>() == 0 {
// REWRITES-NEXT:             return unsafe { std::mem::zeroed() };
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// REWRITES-NEXT:         args[index].read::<T>()
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_identity({{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-AARCH64-GNU-NEXT: fn ext_identity({{arg[0-9]+}}: f128) -> f128 {
// REWRITES-NEXT:     {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_add({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-AARCH64-GNU-NEXT: fn ext_add({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_sub({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-AARCH64-GNU-NEXT: fn ext_sub({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// REWRITES-NEXT:     {{arg[0-9]+}} - {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_mul({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-AARCH64-GNU-NEXT: fn ext_mul({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// REWRITES-NEXT:     {{arg[0-9]+}} * {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_div({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-AARCH64-GNU-NEXT: fn ext_div({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// REWRITES-NEXT:     {{arg[0-9]+}} / {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_neg({{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-AARCH64-GNU-NEXT: fn ext_neg({{arg[0-9]+}}: f128) -> f128 {
// REWRITES-NEXT:     -{{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_eq({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_eq({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// REWRITES-NEXT:     ({{arg[0-9]+}} == {{arg[0-9]+}}) as i32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_lt({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_lt({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// REWRITES-NEXT:     ({{arg[0-9]+}} < {{arg[0-9]+}}) as i32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_le({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_le({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// REWRITES-NEXT:     ({{arg[0-9]+}} <= {{arg[0-9]+}}) as i32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_from_i64({{arg[0-9]+}}: i64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_i64({{arg[0-9]+}})
// REWRITES-AARCH64-GNU-NEXT: fn ext_from_i64({{arg[0-9]+}}: i64) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f128
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_from_u64({{arg[0-9]+}}: u64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_u64({{arg[0-9]+}})
// REWRITES-AARCH64-GNU-NEXT: fn ext_from_u64({{arg[0-9]+}}: u64) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f128
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_to_i64({{arg[0-9]+}}: LongDouble) -> i64 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_i64({{arg[0-9]+}})
// REWRITES-AARCH64-GNU-NEXT: fn ext_to_i64({{arg[0-9]+}}: f128) -> i64 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as i64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_to_u64({{arg[0-9]+}}: LongDouble) -> u64 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_u64({{arg[0-9]+}})
// REWRITES-AARCH64-GNU-NEXT: fn ext_to_u64({{arg[0-9]+}}: f128) -> u64 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_from_double({{arg[0-9]+}}: f64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_f64({{arg[0-9]+}})
// REWRITES-AARCH64-GNU-NEXT: fn ext_from_double({{arg[0-9]+}}: f64) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f128
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_to_double({{arg[0-9]+}}: LongDouble) -> f64 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_f64({{arg[0-9]+}})
// REWRITES-AARCH64-GNU-NEXT: fn ext_to_double({{arg[0-9]+}}: f128) -> f64 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_from_float({{arg[0-9]+}}: f32) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_f32({{arg[0-9]+}})
// REWRITES-AARCH64-GNU-NEXT: fn ext_from_float({{arg[0-9]+}}: f32) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f128
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_to_float({{arg[0-9]+}}: LongDouble) -> f32 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_f32({{arg[0-9]+}})
// REWRITES-AARCH64-GNU-NEXT: fn ext_to_float({{arg[0-9]+}}: f128) -> f32 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_sum10(
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT: ) -> LongDouble {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT: ) -> f128 {
// REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + ({{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}})
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: /// Mix integer, double, float and long-double arguments so each ABI has to
// REWRITES-NEXT: /// classify a nontrivial call correctly.  Volatile temporaries prevent this
// REWRITES-NEXT: /// helper from becoming an accidental constant-expression oracle.
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_mix_abi(
// REWRITES-NEXT:     {{arg[0-9]+}}: i64,
// REWRITES-NEXT:     {{arg[0-9]+}}: f64,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-NEXT:     {{arg[0-9]+}}: u32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-NEXT:     {{arg[0-9]+}}: f32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: f64,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT: ) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT: ) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} - {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i64({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + {{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u32({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} - {{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + {{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} - {{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + {{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} - {{__v[0-9]+}}) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-AARCH64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + ({{arg[0-9]+}} as f128)) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-AARCH64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} - ({{arg[0-9]+}} as f128)) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-AARCH64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + ({{arg[0-9]+}} as f128)) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-AARCH64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} - ({{arg[0-9]+}} as f128)) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-AARCH64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + ({{arg[0-9]+}} as f128)) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-AARCH64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} - ({{arg[0-9]+}} as f128)) };
// REWRITES-NEXT:     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_box_roundtrip(mut b: ld_box) -> ld_box {
// REWRITES-X86_64-GNU-NEXT:     b.x += LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 246, 63]);
// REWRITES-AARCH64-GNU-NEXT: fn ext_box_roundtrip({{arg[0-9]+}}: ld_box) -> ld_box {
// REWRITES-AARCH64-GNU-NEXT:     let mut b: ld_box = ld_box {
// REWRITES-AARCH64-GNU-NEXT:         tag: 0,
// REWRITES-AARCH64-GNU-NEXT:         x: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:         tail: 0,
// REWRITES-AARCH64-GNU-NEXT:     };
// REWRITES-AARCH64-GNU-NEXT:     b = {{arg[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = b.x;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.001953125f128;
// REWRITES-AARCH64-GNU-NEXT:     b.x = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-NEXT:     b.tag = ((b.tag as u32) ^ 90) as u8;
// REWRITES-NEXT:     b.tail ^= 2779077210u32;
// REWRITES-NEXT:     return b;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_pair_make({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> ld_pair {
// REWRITES-AARCH64-GNU-NEXT: fn ext_pair_make({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> ld_pair {
// REWRITES-NEXT:     let mut __retval: ld_pair = ld_pair {
// REWRITES-X86_64-GNU-NEXT:         a: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:         b: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-NEXT:         a: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:         b: 0.0f128,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     __retval.a = {{arg[0-9]+}};
// REWRITES-NEXT:     __retval.b = {{arg[0-9]+}};
// REWRITES-NEXT:     return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_nested_roundtrip(mut n: ld_nested) -> ld_nested {
// REWRITES-X86_64-GNU-NEXT:     n.pair.a += LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:     n.pair.b -= LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     n.z *= LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// REWRITES-AARCH64-GNU-NEXT: fn ext_nested_roundtrip({{arg[0-9]+}}: ld_nested) -> ld_nested {
// REWRITES-AARCH64-GNU-NEXT:     let mut n: ld_nested = ld_nested {
// REWRITES-AARCH64-GNU-NEXT:         head: 0,
// REWRITES-AARCH64-GNU-NEXT:         pair: ld_pair {
// REWRITES-AARCH64-GNU-NEXT:             a: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:             b: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:         },
// REWRITES-AARCH64-GNU-NEXT:         bytes: [0; 3],
// REWRITES-AARCH64-GNU-NEXT:         z: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:     };
// REWRITES-AARCH64-GNU-NEXT:     n = {{arg[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     n.pair.a += {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 2.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     n.pair.b -= {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e-01f128;
// REWRITES-AARCH64-GNU-NEXT:     n.z *= {{__v[0-9]+}};
// REWRITES-NEXT:     n.head = ((n.head as i32) ^ 21930) as u16;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = ((n.bytes[({{__v[0-9]+}} as usize)] as u32) ^ 1) as u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// REWRITES-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = ((n.bytes[({{__v[0-9]+}} as usize)] as u32) ^ 2) as u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// REWRITES-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = ((n.bytes[({{__v[0-9]+}} as usize)] as u32) ^ 4) as u8;
// REWRITES-NEXT:     return n;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_array_sum(mut p: *mut LongDouble, mut n: u64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-AARCH64-GNU-NEXT: unsafe fn ext_array_sum(mut p: *mut f128, mut n: u64) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     r = {{__v[0-9]+}};
// REWRITES-NEXT:     for i in 0..n {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut LongDouble = p;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut LongDouble = unsafe { {{__v[0-9]+}}.add(i as usize) };
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut f128 = p;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut f128 = unsafe { {{__v[0-9]+}}.add(i as usize) };
// REWRITES-NEXT:         r += unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     r
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_store({{arg[0-9]+}}: *mut LongDouble, {{arg[0-9]+}}: LongDouble) {
// REWRITES-AARCH64-GNU-NEXT: unsafe fn ext_store({{arg[0-9]+}}: *mut f128, {{arg[0-9]+}}: f128) {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{arg[0-9]+}} = {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_load({{arg[0-9]+}}: *mut LongDouble) -> LongDouble {
// REWRITES-AARCH64-GNU-NEXT: unsafe fn ext_load({{arg[0-9]+}}: *mut f128) -> f128 {
// REWRITES-NEXT:     unsafe { *{{arg[0-9]+}} }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_call_cb(
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT: ) -> LongDouble {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(f128, f128) -> f128>,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT: ) -> f128 {
// REWRITES-NEXT:     unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_vsum(mut n: i32, mut __slate_va_args: __SlateVaArgs) -> LongDouble {
// REWRITES-AARCH64-GNU-NEXT: unsafe fn ext_vsum(mut n: i32, mut __slate_va_args: __SlateVaArgs) -> f128 {
// REWRITES-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// REWRITES-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     r = {{__v[0-9]+}};
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         ap = __slate_va_args.clone();
// REWRITES-NEXT:     }
// REWRITES-NEXT:     for i in 0..n {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: LongDouble = unsafe { ap.next_arg::<LongDouble>() };
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: f128 = unsafe { ap.next_arg::<f128>() };
// REWRITES-NEXT:         r += {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     r
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_global_set({{arg[0-9]+}}: LongDouble) {
// REWRITES-AARCH64-GNU-NEXT: fn ext_global_set({{arg[0-9]+}}: f128) {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         ext_global_ld = {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_global_get() -> LongDouble {
// REWRITES-AARCH64-GNU-NEXT: fn ext_global_get() -> f128 {
// REWRITES-NEXT:     unsafe { ext_global_ld }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_sizeof_ld() -> u64 {
// REWRITES-NEXT:     16
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_alignof_ld() -> u64 {
// REWRITES-NEXT:     16
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_sizeof_box() -> u64 {
// REWRITES-NEXT:     std::mem::size_of::<ld_box>() as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_alignof_box() -> u64 {
// REWRITES-NEXT:     std::mem::align_of::<ld_box>() as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_offset_box_x() -> u64 {
// REWRITES-NEXT:     std::mem::offset_of!(ld_box, x) as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_offset_box_tail() -> u64 {
// REWRITES-NEXT:     std::mem::offset_of!(ld_box, tail) as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_sizeof_pair() -> u64 {
// REWRITES-NEXT:     std::mem::size_of::<ld_pair>() as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_alignof_pair() -> u64 {
// REWRITES-NEXT:     std::mem::align_of::<ld_pair>() as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_sizeof_nested() -> u64 {
// REWRITES-NEXT:     std::mem::size_of::<ld_nested>() as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_alignof_nested() -> u64 {
// REWRITES-NEXT:     std::mem::align_of::<ld_nested>() as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_offset_nested_pair() -> u64 {
// REWRITES-NEXT:     std::mem::offset_of!(ld_nested, pair) as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_offset_nested_z() -> u64 {
// REWRITES-NEXT:     std::mem::offset_of!(ld_nested, z) as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_sizeof_union() -> u64 {
// REWRITES-NEXT:     std::mem::size_of::<ld_union>() as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn ext_alignof_union() -> u64 {
// REWRITES-NEXT:     std::mem::align_of::<ld_union>() as u64
// REWRITES-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites
