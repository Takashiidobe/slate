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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(f128)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct ld_box {
// COMMON-LOWERING-NEXT:     tag: u8,
// COMMON-LOWERING-NEXT:     tail: u32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct ld_nested {
// COMMON-LOWERING-NEXT:     head: u16,
// COMMON-LOWERING-NEXT:     pair: ld_pair,
// COMMON-LOWERING-NEXT:     bytes: [u8; 3],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct ld_pair {
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union ld_union {
// COMMON-LOWERING-NEXT:     bytes: [u8; 16],
// COMMON-LOWERING-NEXT:     u64: u64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: struct __SlateVaArg {
// COMMON-LOWERING-NEXT:     value: Box<dyn std::any::Any>,
// COMMON-LOWERING-NEXT:     size: usize,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: impl __SlateVaArg {
// COMMON-LOWERING-NEXT:     fn new<T: 'static>(value: T) -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             value: Box::new(value),
// COMMON-LOWERING-NEXT:             size: std::mem::size_of::<T>(),
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// COMMON-LOWERING-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// COMMON-LOWERING-NEXT:             return *value;
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// COMMON-LOWERING-NEXT:         unsafe {
// COMMON-LOWERING-NEXT:             std::ptr::read_unaligned(
// COMMON-LOWERING-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[derive(Clone)]
// COMMON-LOWERING-NEXT: struct __SlateVaArgs {
// COMMON-LOWERING-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// COMMON-LOWERING-NEXT:     index: usize,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: impl __SlateVaArgs {
// COMMON-LOWERING-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             args: Some(std::rc::Rc::new(args)),
// COMMON-LOWERING-NEXT:             index: 0,
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     const fn empty() -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             args: None,
// COMMON-LOWERING-NEXT:             index: 0,
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// COMMON-LOWERING-NEXT:         let index = self.index;
// COMMON-LOWERING-NEXT:         self.index += 1;
// COMMON-LOWERING-NEXT:         if std::mem::size_of::<T>() == 0 {
// COMMON-LOWERING-NEXT:             return unsafe { std::mem::zeroed() };
// COMMON-LOWERING-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// COMMON-LOWERING-NEXT:         args[index].read::<T>()
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{arg[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} == {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} < {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} <= {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_sum10(
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: /// Mix integer, double, float and long-double arguments so each ABI has to
// COMMON-LOWERING-NEXT: /// classify a nontrivial call correctly.  Volatile temporaries prevent this
// COMMON-LOWERING-NEXT: /// helper from becoming an accidental constant-expression oracle.
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_mix_abi(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: f64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: f32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: f64,
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{arg[0-9]+}}) };
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_box_roundtrip({{arg[0-9]+}}: ld_box) -> ld_box {
// COMMON-LOWERING-NEXT:     let mut b: ld_box = ld_box {
// COMMON-LOWERING-NEXT:         tag: 0,
// COMMON-LOWERING-NEXT:         tail: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     b = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     b.x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = b.tag;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 90;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// COMMON-LOWERING-NEXT:     b.tag = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 2779077210u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = b.tail;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     b.tail = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: ld_box = b;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     let mut __retval: ld_pair = ld_pair {
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     __retval.a = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     __retval.b = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: ld_pair = __retval;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_nested_roundtrip({{arg[0-9]+}}: ld_nested) -> ld_nested {
// COMMON-LOWERING-NEXT:     let mut n: ld_nested = ld_nested {
// COMMON-LOWERING-NEXT:         head: 0,
// COMMON-LOWERING-NEXT:         pair: ld_pair {
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         bytes: [0; 3],
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     n = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     n.pair.a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     n.pair.b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     n.z = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 21930;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = n.head;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = {{__v[0-9]+}} as u16;
// COMMON-LOWERING-NEXT:     n.head = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = n.bytes[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// COMMON-LOWERING-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = n.bytes[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// COMMON-LOWERING-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = n.bytes[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// COMMON-LOWERING-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: ld_nested = n;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     let mut n: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut i: u64 = 0;
// COMMON-LOWERING-NEXT:     p = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     n = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     r = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = n;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// COMMON-LOWERING-NEXT:             r = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{arg[0-9]+}} = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_call_cb(
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     let mut n: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// COMMON-LOWERING-NEXT:     let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:     n = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     r = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         ap = __slate_va_args.clone();
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = n;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             r = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         ext_global_ld = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_sizeof_ld() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_alignof_ld() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_sizeof_box() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<ld_box>() as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_alignof_box() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<ld_box>() as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_offset_box_x() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(ld_box, x) as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_offset_box_tail() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(ld_box, tail) as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_sizeof_pair() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<ld_pair>() as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_alignof_pair() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<ld_pair>() as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_sizeof_nested() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<ld_nested>() as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_alignof_nested() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<ld_nested>() as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_offset_nested_pair() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(ld_nested, pair) as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_offset_nested_z() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(ld_nested, z) as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_sizeof_union() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<ld_union>() as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: #[inline(never)]
// COMMON-LOWERING-NEXT: fn ext_alignof_union() -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<ld_union>() as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: #[repr(C, align(16))]
// LOWERING-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
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
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     x: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     z: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     a: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     b: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ld: LongDouble,
// LOWERING-X86_64-GNU-NEXT: static mut ext_global_ld: LongDouble = LongDouble([128, 247, 230, 213, 196, 179, 162, 145, 9, 64]);
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: fn ext_identity({{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT: fn ext_add({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: fn ext_sub({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} - {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: fn ext_mul({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: fn ext_div({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} / {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: fn ext_neg({{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = -{{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: fn ext_eq({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// LOWERING-X86_64-GNU-NEXT: fn ext_lt({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// LOWERING-X86_64-GNU-NEXT: fn ext_le({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// LOWERING-X86_64-GNU-NEXT: fn ext_from_i64({{arg[0-9]+}}: i64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT: fn ext_from_u64({{arg[0-9]+}}: u64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT: fn ext_to_i64({{arg[0-9]+}}: LongDouble) -> i64 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = __slate_f80_to_i64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT: fn ext_to_u64({{arg[0-9]+}}: LongDouble) -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = __slate_f80_to_u64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT: fn ext_from_double({{arg[0-9]+}}: f64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT: fn ext_to_double({{arg[0-9]+}}: LongDouble) -> f64 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT: fn ext_from_float({{arg[0-9]+}}: f32) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT: fn ext_to_float({{arg[0-9]+}}: LongDouble) -> f32 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = __slate_f80_to_f32({{arg[0-9]+}});
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
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT: ) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-X86_64-GNU-NEXT:         x: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = b.x;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 246, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: fn ext_pair_make({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> ld_pair {
// LOWERING-X86_64-GNU-NEXT:         a: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         b: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:             a: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:             b: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         z: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = n.pair.a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = n.pair.b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = n.z;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_array_sum({{arg[0-9]+}}: *mut LongDouble, {{arg[0-9]+}}: u64) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let mut p: *mut LongDouble = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut LongDouble = p;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut LongDouble = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_store({{arg[0-9]+}}: *mut LongDouble, {{arg[0-9]+}}: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_load({{arg[0-9]+}}: *mut LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { *{{arg[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// LOWERING-X86_64-GNU-NEXT: ) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT: unsafe fn ext_vsum({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = unsafe { ap.next_arg::<LongDouble>() };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT: fn ext_global_set({{arg[0-9]+}}: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: fn ext_global_get() -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { ext_global_ld };
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
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
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: struct __va_list {
// LOWERING-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// LOWERING-AARCH64-GNU-NEXT:     x: f128,
// LOWERING-AARCH64-GNU-NEXT:     z: f128,
// LOWERING-AARCH64-GNU-NEXT:     a: f128,
// LOWERING-AARCH64-GNU-NEXT:     b: f128,
// LOWERING-AARCH64-GNU-NEXT: #[repr(C)]
// LOWERING-AARCH64-GNU-NEXT:     ld: f128,
// LOWERING-AARCH64-GNU-NEXT: static mut ext_global_ld: f128 = 1165.0844444444444292230400606058538f128;
// LOWERING-AARCH64-GNU-NEXT: fn ext_identity({{arg[0-9]+}}: f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT: fn ext_add({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: fn ext_sub({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} - {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: fn ext_mul({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: fn ext_div({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} / {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: fn ext_neg({{arg[0-9]+}}: f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = -{{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: fn ext_eq({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn ext_lt({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn ext_le({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn ext_from_i64({{arg[0-9]+}}: i64) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT: fn ext_from_u64({{arg[0-9]+}}: u64) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT: fn ext_to_i64({{arg[0-9]+}}: f128) -> i64 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = {{arg[0-9]+}} as i64;
// LOWERING-AARCH64-GNU-NEXT: fn ext_to_u64({{arg[0-9]+}}: f128) -> u64 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// LOWERING-AARCH64-GNU-NEXT: fn ext_from_double({{arg[0-9]+}}: f64) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT: fn ext_to_double({{arg[0-9]+}}: f128) -> f64 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = {{arg[0-9]+}} as f64;
// LOWERING-AARCH64-GNU-NEXT: fn ext_from_float({{arg[0-9]+}}: f32) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT: fn ext_to_float({{arg[0-9]+}}: f128) -> f32 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{arg[0-9]+}} as f32;
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
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT: ) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-AARCH64-GNU-NEXT:         x: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = b.x;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.001953125f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: fn ext_pair_make({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> ld_pair {
// LOWERING-AARCH64-GNU-NEXT:         a: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:         b: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:             a: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:             b: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:         z: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = n.pair.a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 2.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = n.pair.b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e-01f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = n.z;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: unsafe fn ext_array_sum({{arg[0-9]+}}: *mut f128, {{arg[0-9]+}}: u64) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let mut p: *mut f128 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut f128 = p;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut f128 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = r;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = r;
// LOWERING-AARCH64-GNU-NEXT: unsafe fn ext_store({{arg[0-9]+}}: *mut f128, {{arg[0-9]+}}: f128) {
// LOWERING-AARCH64-GNU-NEXT: unsafe fn ext_load({{arg[0-9]+}}: *mut f128) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { *{{arg[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(f128, f128) -> f128>,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-AARCH64-GNU-NEXT: ) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-AARCH64-GNU-NEXT: unsafe fn ext_vsum({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = unsafe { ap.next_arg::<f128>() };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = r;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = r;
// LOWERING-AARCH64-GNU-NEXT: fn ext_global_set({{arg[0-9]+}}: f128) {
// LOWERING-AARCH64-GNU-NEXT: fn ext_global_get() -> f128 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { ext_global_ld };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(f128)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct ld_box {
// COMMON-REWRITES-NEXT:     tag: u8,
// COMMON-REWRITES-NEXT:     tail: u32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct ld_nested {
// COMMON-REWRITES-NEXT:     head: u16,
// COMMON-REWRITES-NEXT:     pair: ld_pair,
// COMMON-REWRITES-NEXT:     bytes: [u8; 3],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct ld_pair {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union ld_union {
// COMMON-REWRITES-NEXT:     bytes: [u8; 16],
// COMMON-REWRITES-NEXT:     u64: u64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: struct __SlateVaArg {
// COMMON-REWRITES-NEXT:     value: Box<dyn std::any::Any>,
// COMMON-REWRITES-NEXT:     size: usize,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: impl __SlateVaArg {
// COMMON-REWRITES-NEXT:     fn new<T: 'static>(value: T) -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             value: Box::new(value),
// COMMON-REWRITES-NEXT:             size: std::mem::size_of::<T>(),
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// COMMON-REWRITES-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// COMMON-REWRITES-NEXT:             return *value;
// COMMON-REWRITES-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             std::ptr::read_unaligned(
// COMMON-REWRITES-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[derive(Clone)]
// COMMON-REWRITES-NEXT: struct __SlateVaArgs {
// COMMON-REWRITES-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// COMMON-REWRITES-NEXT:     index: usize,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: impl __SlateVaArgs {
// COMMON-REWRITES-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             args: Some(std::rc::Rc::new(args)),
// COMMON-REWRITES-NEXT:             index: 0,
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     const fn empty() -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             args: None,
// COMMON-REWRITES-NEXT:             index: 0,
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// COMMON-REWRITES-NEXT:         let index = self.index;
// COMMON-REWRITES-NEXT:         self.index += 1;
// COMMON-REWRITES-NEXT:         if std::mem::size_of::<T>() == 0 {
// COMMON-REWRITES-NEXT:             return unsafe { std::mem::zeroed() };
// COMMON-REWRITES-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// COMMON-REWRITES-NEXT:         args[index].read::<T>()
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} - {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} * {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} / {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     -{{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     ({{arg[0-9]+}} == {{arg[0-9]+}}) as i32
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     ({{arg[0-9]+}} < {{arg[0-9]+}}) as i32
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     ({{arg[0-9]+}} <= {{arg[0-9]+}}) as i32
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_sum10(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + ({{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}})
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: /// Mix integer, double, float and long-double arguments so each ABI has to
// COMMON-REWRITES-NEXT: /// classify a nontrivial call correctly.  Volatile temporaries prevent this
// COMMON-REWRITES-NEXT: /// helper from becoming an accidental constant-expression oracle.
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_mix_abi(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i64,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: f64,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: u32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: f32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: f64,
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{arg[0-9]+}}) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + {{arg[0-9]+}}) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} - {{arg[0-9]+}}) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + {{arg[0-9]+}}) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     b.tag = ((b.tag as u32) ^ 90) as u8;
// COMMON-REWRITES-NEXT:     b.tail ^= 2779077210u32;
// COMMON-REWRITES-NEXT:     return b;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     let mut __retval: ld_pair = ld_pair {
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     __retval.a = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     __retval.b = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     return __retval;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     n.head = ((n.head as i32) ^ 21930) as u16;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-REWRITES-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = ((n.bytes[({{__v[0-9]+}} as usize)] as u32) ^ 1) as u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-REWRITES-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = ((n.bytes[({{__v[0-9]+}} as usize)] as u32) ^ 2) as u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-REWRITES-NEXT:     n.bytes[({{__v[0-9]+}} as usize)] = ((n.bytes[({{__v[0-9]+}} as usize)] as u32) ^ 4) as u8;
// COMMON-REWRITES-NEXT:     return n;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     for i in 0..n {
// COMMON-REWRITES-NEXT:         r += unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     r
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{arg[0-9]+}} = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     unsafe { *{{arg[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_call_cb(
// COMMON-REWRITES-NEXT:     unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         ap = __slate_va_args.clone();
// COMMON-REWRITES-NEXT:     for i in 0..n {
// COMMON-REWRITES-NEXT:         r += {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     r
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         ext_global_ld = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT:     unsafe { ext_global_ld }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_sizeof_ld() -> u64 {
// COMMON-REWRITES-NEXT:     16
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_alignof_ld() -> u64 {
// COMMON-REWRITES-NEXT:     16
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_sizeof_box() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::size_of::<ld_box>() as u64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_alignof_box() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::align_of::<ld_box>() as u64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_offset_box_x() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::offset_of!(ld_box, x) as u64
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_offset_box_tail() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::offset_of!(ld_box, tail) as u64
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_sizeof_pair() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::size_of::<ld_pair>() as u64
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_alignof_pair() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::align_of::<ld_pair>() as u64
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_sizeof_nested() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::size_of::<ld_nested>() as u64
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_alignof_nested() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::align_of::<ld_nested>() as u64
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_offset_nested_pair() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::offset_of!(ld_nested, pair) as u64
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_offset_nested_z() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::offset_of!(ld_nested, z) as u64
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_sizeof_union() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::size_of::<ld_union>() as u64
// COMMON-REWRITES-NEXT: #[inline(never)]
// COMMON-REWRITES-NEXT: fn ext_alignof_union() -> u64 {
// COMMON-REWRITES-NEXT:     std::mem::align_of::<ld_union>() as u64
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: #[repr(C, align(16))]
// REWRITES-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
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
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     x: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     z: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     a: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     b: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ld: LongDouble,
// REWRITES-X86_64-GNU-NEXT: static mut ext_global_ld: LongDouble = LongDouble([128, 247, 230, 213, 196, 179, 162, 145, 9, 64]);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: fn ext_identity({{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT: fn ext_add({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT: fn ext_sub({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT: fn ext_mul({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT: fn ext_div({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT: fn ext_neg({{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT: fn ext_eq({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// REWRITES-X86_64-GNU-NEXT: fn ext_lt({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// REWRITES-X86_64-GNU-NEXT: fn ext_le({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> i32 {
// REWRITES-X86_64-GNU-NEXT: fn ext_from_i64({{arg[0-9]+}}: i64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_i64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: fn ext_from_u64({{arg[0-9]+}}: u64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_u64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: fn ext_to_i64({{arg[0-9]+}}: LongDouble) -> i64 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_i64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: fn ext_to_u64({{arg[0-9]+}}: LongDouble) -> u64 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_u64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: fn ext_from_double({{arg[0-9]+}}: f64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_f64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: fn ext_to_double({{arg[0-9]+}}: LongDouble) -> f64 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_f64({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: fn ext_from_float({{arg[0-9]+}}: f32) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_from_f32({{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: fn ext_to_float({{arg[0-9]+}}: LongDouble) -> f32 {
// REWRITES-X86_64-GNU-NEXT:     __slate_f80_to_f32({{arg[0-9]+}})
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
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT: ) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
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
// REWRITES-X86_64-GNU-NEXT: fn ext_box_roundtrip(mut b: ld_box) -> ld_box {
// REWRITES-X86_64-GNU-NEXT:     b.x += LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 246, 63]);
// REWRITES-X86_64-GNU-NEXT: fn ext_pair_make({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> ld_pair {
// REWRITES-X86_64-GNU-NEXT:         a: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:         b: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT: fn ext_nested_roundtrip(mut n: ld_nested) -> ld_nested {
// REWRITES-X86_64-GNU-NEXT:     n.pair.a += LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:     n.pair.b -= LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     n.z *= LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_array_sum(mut p: *mut LongDouble, mut n: u64) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut LongDouble = p;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut LongDouble = unsafe { {{__v[0-9]+}}.add(i as usize) };
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_store({{arg[0-9]+}}: *mut LongDouble, {{arg[0-9]+}}: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_load({{arg[0-9]+}}: *mut LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: LongDouble,
// REWRITES-X86_64-GNU-NEXT: ) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT: unsafe fn ext_vsum(mut n: i32, mut __slate_va_args: __SlateVaArgs) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: LongDouble = unsafe { ap.next_arg::<LongDouble>() };
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: fn ext_global_set({{arg[0-9]+}}: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: fn ext_global_get() -> LongDouble {
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
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
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: struct __va_list {
// REWRITES-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// REWRITES-AARCH64-GNU-NEXT:     x: f128,
// REWRITES-AARCH64-GNU-NEXT:     z: f128,
// REWRITES-AARCH64-GNU-NEXT:     a: f128,
// REWRITES-AARCH64-GNU-NEXT:     b: f128,
// REWRITES-AARCH64-GNU-NEXT: #[repr(C)]
// REWRITES-AARCH64-GNU-NEXT:     ld: f128,
// REWRITES-AARCH64-GNU-NEXT: static mut ext_global_ld: f128 = 1165.0844444444444292230400606058538f128;
// REWRITES-AARCH64-GNU-NEXT: fn ext_identity({{arg[0-9]+}}: f128) -> f128 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_add({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_sub({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_mul({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_div({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_neg({{arg[0-9]+}}: f128) -> f128 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_eq({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_lt({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_le({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn ext_from_i64({{arg[0-9]+}}: i64) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f128
// REWRITES-AARCH64-GNU-NEXT: fn ext_from_u64({{arg[0-9]+}}: u64) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f128
// REWRITES-AARCH64-GNU-NEXT: fn ext_to_i64({{arg[0-9]+}}: f128) -> i64 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as i64
// REWRITES-AARCH64-GNU-NEXT: fn ext_to_u64({{arg[0-9]+}}: f128) -> u64 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as u64
// REWRITES-AARCH64-GNU-NEXT: fn ext_from_double({{arg[0-9]+}}: f64) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f128
// REWRITES-AARCH64-GNU-NEXT: fn ext_to_double({{arg[0-9]+}}: f128) -> f64 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f64
// REWRITES-AARCH64-GNU-NEXT: fn ext_from_float({{arg[0-9]+}}: f32) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f128
// REWRITES-AARCH64-GNU-NEXT: fn ext_to_float({{arg[0-9]+}}: f128) -> f32 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} as f32
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
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT: ) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
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
// REWRITES-AARCH64-GNU-NEXT: fn ext_box_roundtrip({{arg[0-9]+}}: ld_box) -> ld_box {
// REWRITES-AARCH64-GNU-NEXT:     let mut b: ld_box = ld_box {
// REWRITES-AARCH64-GNU-NEXT:         tag: 0,
// REWRITES-AARCH64-GNU-NEXT:         x: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:         tail: 0,
// REWRITES-AARCH64-GNU-NEXT:     b = {{arg[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = b.x;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.001953125f128;
// REWRITES-AARCH64-GNU-NEXT:     b.x = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT: fn ext_pair_make({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> ld_pair {
// REWRITES-AARCH64-GNU-NEXT:         a: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:         b: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:     };
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
// REWRITES-AARCH64-GNU-NEXT: unsafe fn ext_array_sum(mut p: *mut f128, mut n: u64) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     r = {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut f128 = p;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut f128 = unsafe { {{__v[0-9]+}}.add(i as usize) };
// REWRITES-AARCH64-GNU-NEXT: unsafe fn ext_store({{arg[0-9]+}}: *mut f128, {{arg[0-9]+}}: f128) {
// REWRITES-AARCH64-GNU-NEXT: unsafe fn ext_load({{arg[0-9]+}}: *mut f128) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(f128, f128) -> f128>,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: f128,
// REWRITES-AARCH64-GNU-NEXT: ) -> f128 {
// REWRITES-AARCH64-GNU-NEXT: unsafe fn ext_vsum(mut n: i32, mut __slate_va_args: __SlateVaArgs) -> f128 {
// REWRITES-AARCH64-GNU-NEXT:     let mut r: f128 = 0.0f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     r = {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: f128 = unsafe { ap.next_arg::<f128>() };
// REWRITES-AARCH64-GNU-NEXT: fn ext_global_set({{arg[0-9]+}}: f128) {
// REWRITES-AARCH64-GNU-NEXT: fn ext_global_get() -> f128 {
// SLATE-FILECHECK-END rewrites-aarch64-gnu
