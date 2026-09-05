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

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU: #![allow(
// LOWERING-X86_64-GNU-NEXT:     dead_code,
// LOWERING-X86_64-GNU-NEXT:     unused,
// LOWERING-X86_64-GNU-NEXT:     non_camel_case_types,
// LOWERING-X86_64-GNU-NEXT:     non_snake_case,
// LOWERING-X86_64-GNU-NEXT:     non_upper_case_globals,
// LOWERING-X86_64-GNU-NEXT:     arithmetic_overflow,
// LOWERING-X86_64-GNU-NEXT:     unconditional_panic,
// LOWERING-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-X86_64-GNU-NEXT:     unused_comparisons
// LOWERING-X86_64-GNU-NEXT: )]
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C, align(16))]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn add(self, o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_add(self, o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn sub(self, o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_sub(self, o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn mul(self, o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_mul(self, o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn div(self, o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_div(self, o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn add_assign(&mut self, o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn sub_assign(&mut self, o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn mul_assign(&mut self, o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn div_assign(&mut self, o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, o);
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
// LOWERING-X86_64-GNU-NEXT:     fn eq(&self, other: &LongDouble) -> bool {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *other)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *other) {
// LOWERING-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-X86_64-GNU-NEXT:         } else {
// LOWERING-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *other) {
// LOWERING-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-X86_64-GNU-NEXT:             } else {
// LOWERING-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *other) {
// LOWERING-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// LOWERING-X86_64-GNU-NEXT:                 } else {
// LOWERING-X86_64-GNU-NEXT:                     None
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct ld_box {
// LOWERING-X86_64-GNU-NEXT:     tag: u8,
// LOWERING-X86_64-GNU-NEXT:     x: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     tail: u32,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct ld_nested {
// LOWERING-X86_64-GNU-NEXT:     head: u16,
// LOWERING-X86_64-GNU-NEXT:     pair: ld_pair,
// LOWERING-X86_64-GNU-NEXT:     bytes: [u8; 3],
// LOWERING-X86_64-GNU-NEXT:     z: LongDouble,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct ld_pair {
// LOWERING-X86_64-GNU-NEXT:     a: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     b: LongDouble,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: union ld_union {
// LOWERING-X86_64-GNU-NEXT:     ld: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     bytes: [u8; 16],
// LOWERING-X86_64-GNU-NEXT:     u64: u64,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: static mut ext_global_ld: LongDouble = LongDouble([128, 247, 230, 213, 196, 179, 162, 145, 9, 64]);
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: struct __SlateVaArg {
// LOWERING-X86_64-GNU-NEXT:     value: Box<dyn std::any::Any>,
// LOWERING-X86_64-GNU-NEXT:     size: usize,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl __SlateVaArg {
// LOWERING-X86_64-GNU-NEXT:     fn new<T: 'static>(value: T) -> Self {
// LOWERING-X86_64-GNU-NEXT:         Self {
// LOWERING-X86_64-GNU-NEXT:             value: Box::new(value),
// LOWERING-X86_64-GNU-NEXT:             size: std::mem::size_of::<T>(),
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// LOWERING-X86_64-GNU-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// LOWERING-X86_64-GNU-NEXT:             return *value;
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// LOWERING-X86_64-GNU-NEXT:         unsafe {
// LOWERING-X86_64-GNU-NEXT:             std::ptr::read_unaligned(
// LOWERING-X86_64-GNU-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// LOWERING-X86_64-GNU-NEXT:             )
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone)]
// LOWERING-X86_64-GNU-NEXT: struct __SlateVaArgs {
// LOWERING-X86_64-GNU-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// LOWERING-X86_64-GNU-NEXT:     index: usize,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl __SlateVaArgs {
// LOWERING-X86_64-GNU-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// LOWERING-X86_64-GNU-NEXT:         Self {
// LOWERING-X86_64-GNU-NEXT:             args: Some(std::rc::Rc::new(args)),
// LOWERING-X86_64-GNU-NEXT:             index: 0,
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     const fn empty() -> Self {
// LOWERING-X86_64-GNU-NEXT:         Self {
// LOWERING-X86_64-GNU-NEXT:             args: None,
// LOWERING-X86_64-GNU-NEXT:             index: 0,
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// LOWERING-X86_64-GNU-NEXT:         let index = self.index;
// LOWERING-X86_64-GNU-NEXT:         self.index += 1;
// LOWERING-X86_64-GNU-NEXT:         if std::mem::size_of::<T>() == 0 {
// LOWERING-X86_64-GNU-NEXT:             return unsafe { std::mem::zeroed() };
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// LOWERING-X86_64-GNU-NEXT:         args[index].read::<T>()
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_identity({{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     return {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_add({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_sub({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{arg[0-9]+}} - {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_mul({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_div({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{arg[0-9]+}} / {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_neg({{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = -{{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_eq({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} == {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_lt({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} < {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_le({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} <= {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_from_i64({{arg[0-9]+}}: i64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_from_u64({{arg[0-9]+}}: u64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_to_i64({{arg[0-9]+}}: LongDouble) -> i64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = __slate_f80_to_i64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_to_u64({{arg[0-9]+}}: LongDouble) -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = __slate_f80_to_u64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_from_double({{arg[0-9]+}}: f64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_to_double({{arg[0-9]+}}: LongDouble) -> f64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_from_float({{arg[0-9]+}}: f32) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_to_float({{arg[0-9]+}}: LongDouble) -> f32 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: f32 = __slate_f80_to_f32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_sum10(
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
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: /// Mix integer, double, float and long-double arguments so each ABI has to
// LOWERING-X86_64-GNU-NEXT: /// classify a nontrivial call correctly.  Volatile temporaries prevent this
// LOWERING-X86_64-GNU-NEXT: /// helper from becoming an accidental constant-expression oracle.
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_mix_abi(
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i64,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: f64,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: u32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: f32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: f64,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT: ) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_box_roundtrip({{arg[0-9]+}}: ld_box) -> ld_box {
// LOWERING-X86_64-GNU-NEXT:     let mut b: ld_box = ld_box {
// LOWERING-X86_64-GNU-NEXT:         tag: 0,
// LOWERING-X86_64-GNU-NEXT:         x: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         tail: 0,
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     b = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = b.x;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 246, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     b.x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = b.tag;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = 90;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-X86_64-GNU-NEXT:     b.tag = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = 2779077210u32;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = b.tail;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     b.tail = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: ld_box = b;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_pair_make({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> ld_pair {
// LOWERING-X86_64-GNU-NEXT:     let mut __retval: ld_pair = ld_pair {
// LOWERING-X86_64-GNU-NEXT:         a: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         b: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     __retval.a = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     __retval.b = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: ld_pair = __retval;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_nested_roundtrip({{arg[0-9]+}}: ld_nested) -> ld_nested {
// LOWERING-X86_64-GNU-NEXT:     let mut n: ld_nested = ld_nested {
// LOWERING-X86_64-GNU-NEXT:         head: 0,
// LOWERING-X86_64-GNU-NEXT:         pair: ld_pair {
// LOWERING-X86_64-GNU-NEXT:             a: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:             b: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         },
// LOWERING-X86_64-GNU-NEXT:         bytes: [0; 3],
// LOWERING-X86_64-GNU-NEXT:         z: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     n = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = n.pair.a;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     n.pair.a = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = n.pair.b;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     n.pair.b = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = n.z;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     n.z = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = 21930;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u16 = n.head;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u16 = {{_v[0-9]+}} as u16;
// LOWERING-X86_64-GNU-NEXT:     n.head = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = n.bytes[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = n.bytes[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = 4;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = n.bytes[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: ld_nested = n;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_array_sum({{arg[0-9]+}}: *mut LongDouble, {{arg[0-9]+}}: u64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let mut p: *mut LongDouble = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut n: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut i: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:     p = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     n = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     r = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:         i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = n;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: *mut LongDouble = p;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: *mut LongDouble = unsafe { {{_v[0-9]+}}.add({{_v[0-9]+}} as usize) };
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = unsafe { *{{_v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             r = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:             i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_store({{arg[0-9]+}}: *mut LongDouble, {{arg[0-9]+}}: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         *{{arg[0-9]+}} = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_load({{arg[0-9]+}}: *mut LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { *{{arg[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_call_cb(
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT: ) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_vsum({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let mut n: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut i: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     n = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     r = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         ap = __slate_va_args.clone();
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:         i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = n;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = unsafe { ap.next_arg::<LongDouble>() };
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             r = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:             i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_global_set({{arg[0-9]+}}: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         ext_global_ld = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_global_get() -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { ext_global_ld };
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_sizeof_ld() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_alignof_ld() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_sizeof_box() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_box>() as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_alignof_box() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_box>() as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_offset_box_x() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::offset_of!(ld_box, x) as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_offset_box_tail() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::offset_of!(ld_box, tail) as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_sizeof_pair() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_pair>() as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_alignof_pair() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_pair>() as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_sizeof_nested() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_nested>() as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_alignof_nested() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_nested>() as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_offset_nested_pair() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::offset_of!(ld_nested, pair) as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_offset_nested_z() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::offset_of!(ld_nested, z) as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_sizeof_union() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_union>() as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn ext_alignof_union() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_union>() as u64;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// LOWERING-X86_64-GNU-NEXT:         a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-X86_64-GNU-NEXT:         a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_add(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_div(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(a: LongDouble, b: LongDouble, c: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(a: bool) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(a: f32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(a: f64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(a: i128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(a: i16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(a: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(a: i64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(a: i8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(a: u128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(a: u16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(a: u32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(a: u64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(a: u8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(a: LongDouble, flags: i32) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_le(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_round(a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(a: LongDouble) -> f32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(a: LongDouble) -> f64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(a: LongDouble) -> i128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(a: LongDouble) -> i16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(a: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(a: LongDouble) -> i64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(a: LongDouble) -> i8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(a: LongDouble) -> u128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(a: LongDouble) -> u16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(a: LongDouble) -> u32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(a: LongDouble) -> u64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(a: LongDouble) -> u8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU: #![allow(
// REWRITES-X86_64-GNU-NEXT:     dead_code,
// REWRITES-X86_64-GNU-NEXT:     unused,
// REWRITES-X86_64-GNU-NEXT:     non_camel_case_types,
// REWRITES-X86_64-GNU-NEXT:     non_snake_case,
// REWRITES-X86_64-GNU-NEXT:     non_upper_case_globals,
// REWRITES-X86_64-GNU-NEXT:     arithmetic_overflow,
// REWRITES-X86_64-GNU-NEXT:     unconditional_panic,
// REWRITES-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-X86_64-GNU-NEXT:     unused_comparisons
// REWRITES-X86_64-GNU-NEXT: )]
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C, align(16))]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn add(self, o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_add(self, o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn sub(self, o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_sub(self, o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn mul(self, o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_mul(self, o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn div(self, o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_div(self, o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn add_assign(&mut self, o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn sub_assign(&mut self, o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn mul_assign(&mut self, o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn div_assign(&mut self, o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, o);
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
// REWRITES-X86_64-GNU-NEXT:     fn eq(&self, other: &LongDouble) -> bool {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *other)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *other) {
// REWRITES-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *other) {
// REWRITES-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *other) {
// REWRITES-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-X86_64-GNU-NEXT:                 } else {
// REWRITES-X86_64-GNU-NEXT:                     None
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct ld_box {
// REWRITES-X86_64-GNU-NEXT:     tag: u8,
// REWRITES-X86_64-GNU-NEXT:     x: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     tail: u32,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct ld_nested {
// REWRITES-X86_64-GNU-NEXT:     head: u16,
// REWRITES-X86_64-GNU-NEXT:     pair: ld_pair,
// REWRITES-X86_64-GNU-NEXT:     bytes: [u8; 3],
// REWRITES-X86_64-GNU-NEXT:     z: LongDouble,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct ld_pair {
// REWRITES-X86_64-GNU-NEXT:     a: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     b: LongDouble,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: union ld_union {
// REWRITES-X86_64-GNU-NEXT:     ld: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     bytes: [u8; 16],
// REWRITES-X86_64-GNU-NEXT:     u64: u64,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: static mut ext_global_ld: LongDouble = LongDouble([128, 247, 230, 213, 196, 179, 162, 145, 9, 64]);
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: struct __SlateVaArg {
// REWRITES-X86_64-GNU-NEXT:     value: Box<dyn std::any::Any>,
// REWRITES-X86_64-GNU-NEXT:     size: usize,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl __SlateVaArg {
// REWRITES-X86_64-GNU-NEXT:     fn new<T: 'static>(value: T) -> Self {
// REWRITES-X86_64-GNU-NEXT:         Self {
// REWRITES-X86_64-GNU-NEXT:             value: Box::new(value),
// REWRITES-X86_64-GNU-NEXT:             size: std::mem::size_of::<T>(),
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// REWRITES-X86_64-GNU-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// REWRITES-X86_64-GNU-NEXT:             return *value;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             std::ptr::read_unaligned(
// REWRITES-X86_64-GNU-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// REWRITES-X86_64-GNU-NEXT:             )
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone)]
// REWRITES-X86_64-GNU-NEXT: struct __SlateVaArgs {
// REWRITES-X86_64-GNU-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// REWRITES-X86_64-GNU-NEXT:     index: usize,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl __SlateVaArgs {
// REWRITES-X86_64-GNU-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// REWRITES-X86_64-GNU-NEXT:         Self {
// REWRITES-X86_64-GNU-NEXT:             args: Some(std::rc::Rc::new(args)),
// REWRITES-X86_64-GNU-NEXT:             index: 0,
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     const fn empty() -> Self {
// REWRITES-X86_64-GNU-NEXT:         Self {
// REWRITES-X86_64-GNU-NEXT:             args: None,
// REWRITES-X86_64-GNU-NEXT:             index: 0,
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// REWRITES-X86_64-GNU-NEXT:         let index = self.index;
// REWRITES-X86_64-GNU-NEXT:         self.index += 1;
// REWRITES-X86_64-GNU-NEXT:         if std::mem::size_of::<T>() == 0 {
// REWRITES-X86_64-GNU-NEXT:             return unsafe { std::mem::zeroed() };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// REWRITES-X86_64-GNU-NEXT:         args[index].read::<T>()
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_identity({{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_add({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_sub({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}} - {{arg[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_mul({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}} * {{arg[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_div({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}} / {{arg[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_neg({{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     -{{arg[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_eq({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// REWRITES-X86_64-GNU-NEXT:     ({{arg[0-9]+}} == {{arg[0-9]+}}) as i32
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_lt({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// REWRITES-X86_64-GNU-NEXT:     ({{arg[0-9]+}} < {{arg[0-9]+}}) as i32
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_le({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// REWRITES-X86_64-GNU-NEXT:     ({{arg[0-9]+}} <= {{arg[0-9]+}}) as i32
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_from_i64({{arg[0-9]+}}: i64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_i64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_from_u64({{arg[0-9]+}}: u64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_u64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_to_i64({{arg[0-9]+}}: LongDouble) -> i64 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_i64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_to_u64({{arg[0-9]+}}: LongDouble) -> u64 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_u64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_from_double({{arg[0-9]+}}: f64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_f64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_to_double({{arg[0-9]+}}: LongDouble) -> f64 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_f64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_from_float({{arg[0-9]+}}: f32) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_f32({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_to_float({{arg[0-9]+}}: LongDouble) -> f32 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_f32({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_sum10(
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
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + ({{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: /// Mix integer, double, float and long-double arguments so each ABI has to
// REWRITES-X86_64-GNU-NEXT: /// classify a nontrivial call correctly.  Volatile temporaries prevent this
// REWRITES-X86_64-GNU-NEXT: /// helper from becoming an accidental constant-expression oracle.
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_mix_abi(
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i64,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: f64,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: u32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: f32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: f64,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT: ) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}} + {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}} - {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}} + {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i64({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u32({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}} - {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}} - {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{_v[0-9]+}} - {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_box_roundtrip(mut b: ld_box) -> ld_box {
// REWRITES-X86_64-GNU-NEXT:     b.x += LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 246, 63]);
// REWRITES-X86_64-GNU-NEXT:     b.tag = ((b.tag as u32) ^ 90) as u8;
// REWRITES-X86_64-GNU-NEXT:     b.tail ^= 2779077210u32;
// REWRITES-X86_64-GNU-NEXT:     return b;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_pair_make({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> ld_pair {
// REWRITES-X86_64-GNU-NEXT:     let mut __retval: ld_pair = ld_pair {
// REWRITES-X86_64-GNU-NEXT:         a: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:         b: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     __retval.a = {{arg[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     __retval.b = {{arg[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     return __retval;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_nested_roundtrip(mut n: ld_nested) -> ld_nested {
// REWRITES-X86_64-GNU-NEXT:     n.pair.a += LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:     n.pair.b -= LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     n.z *= LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// REWRITES-X86_64-GNU-NEXT:     n.head = ((n.head as i32) ^ 21930) as u16;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// REWRITES-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = ((n.bytes[({{_v[0-9]+}} as usize)] as u32) ^ 1) as u8;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// REWRITES-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = ((n.bytes[({{_v[0-9]+}} as usize)] as u32) ^ 2) as u8;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// REWRITES-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = ((n.bytes[({{_v[0-9]+}} as usize)] as u32) ^ 4) as u8;
// REWRITES-X86_64-GNU-NEXT:     return n;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_array_sum(mut p: *mut LongDouble, mut n: u64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:     for i in 0..n {
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: *mut LongDouble = p;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: *mut LongDouble = unsafe { {{_v[0-9]+}}.add(i as usize) };
// REWRITES-X86_64-GNU-NEXT:         r += unsafe { *{{_v[0-9]+}} };
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     r
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_store({{arg[0-9]+}}: *mut LongDouble, {{arg[0-9]+}}: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         *{{arg[0-9]+}} = {{arg[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_load({{arg[0-9]+}}: *mut LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     unsafe { *{{arg[0-9]+}} }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_call_cb(
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT: ) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_vsum(mut n: i32, mut __slate_va_args: __SlateVaArgs) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// REWRITES-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         ap = __slate_va_args.clone();
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     for i in 0..n {
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { ap.next_arg::<LongDouble>() };
// REWRITES-X86_64-GNU-NEXT:         r += {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     r
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_global_set({{arg[0-9]+}}: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         ext_global_ld = {{arg[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_global_get() -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     unsafe { ext_global_ld }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_sizeof_ld() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     16
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_alignof_ld() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     16
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_sizeof_box() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::size_of::<ld_box>() as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_alignof_box() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::align_of::<ld_box>() as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_offset_box_x() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::offset_of!(ld_box, x) as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_offset_box_tail() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::offset_of!(ld_box, tail) as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_sizeof_pair() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::size_of::<ld_pair>() as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_alignof_pair() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::align_of::<ld_pair>() as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_sizeof_nested() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::size_of::<ld_nested>() as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_alignof_nested() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::align_of::<ld_nested>() as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_offset_nested_pair() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::offset_of!(ld_nested, pair) as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_offset_nested_z() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::offset_of!(ld_nested, z) as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_sizeof_union() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::size_of::<ld_union>() as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn ext_alignof_union() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     std::mem::align_of::<ld_union>() as u64
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// REWRITES-X86_64-GNU-NEXT:         a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-X86_64-GNU-NEXT:         a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_add(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_div(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(a: LongDouble, b: LongDouble, c: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(a: bool) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(a: f32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(a: f64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(a: i128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(a: i16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(a: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(a: i64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(a: i8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(a: u128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(a: u16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(a: u32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(a: u64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(a: u8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(a: LongDouble, flags: i32) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_le(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_round(a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(a: LongDouble) -> f32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(a: LongDouble) -> f64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(a: LongDouble) -> i128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(a: LongDouble) -> i16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(a: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(a: LongDouble) -> i64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(a: LongDouble) -> i8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(a: LongDouble) -> u128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(a: LongDouble) -> u16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(a: LongDouble) -> u32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(a: LongDouble) -> u64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(a: LongDouble) -> u8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu
