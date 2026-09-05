#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct padded {
  unsigned char byte;
  unsigned int  word;
};

struct bit_padded {
  unsigned char low  : 3;
  unsigned char      : 2;
  unsigned char high : 3;
};

typedef double double2 __attribute__((ext_vector_type(2)));

static int cache_prefetch_probe(int x) {
  char bytes[16] = {0};
  bytes[0]       = (char)x;
  __builtin___clear_cache(bytes, bytes + sizeof(bytes));
  __builtin_prefetch(bytes + 1, 0, 3);
  return bytes[0] + 1;
}

static int frame_probe(void) {
  void *frame = __builtin_frame_address(0);
  return frame != 0;
}

static int clear_padding_probe(void) {
  struct padded     value;
  struct bit_padded bits;
  memset(&value, 0xff, sizeof(value));
  memset(&bits, 0xff, sizeof(bits));
  value.byte = 7;
  value.word = 11;
  bits.low   = 7;
  bits.high  = 7;
#if __has_builtin(__builtin_clear_padding)
  __builtin_clear_padding(&value);
  __builtin_clear_padding(&bits);
#else
  unsigned char *padding   = (unsigned char *)&value;
  padding[1]               = 0;
  padding[2]               = 0;
  padding[3]               = 0;
  *(unsigned char *)&bits &= 0xe7;
#endif
  unsigned char *bytes = (unsigned char *)&value;
  return 10 * (bytes[1] + bytes[2] + bytes[3]) +
         (*(unsigned char *)&bits == 0xe7);
}

static int frexp_probe(void) {
  volatile double input   = 12.0;
  volatile float  input_f = 8.0f;
  int             exponent_d;
  int             exponent_f;
  double          fraction_d = __builtin_frexp(input, &exponent_d);
  float           fraction_f = __builtin_frexpf(input_f, &exponent_f);
  return 100 * (10 * (fraction_d == 0.75) + exponent_d) +
         10 * (fraction_f == 0.5f) + exponent_f;
}

static int hyperbolic_probe(void) {
  volatile double input = 0.0;
#if __has_builtin(__builtin_elementwise_cosh)
  double           c            = __builtin_elementwise_cosh(input);
  double           s            = __builtin_elementwise_sinh(input);
  double           t            = __builtin_elementwise_tanh(input);
  volatile double2 vector_input = {0.0, 0.0};
  double2          vc           = __builtin_elementwise_cosh(vector_input);
  double2          vs           = __builtin_elementwise_sinh(vector_input);
  double2          vt           = __builtin_elementwise_tanh(vector_input);
  int              vector_ok = vc[0] == 1.0 && vc[1] == 1.0 && vs[0] == 0.0 &&
                               vs[1] == 0.0 && vt[0] == 0.0 && vt[1] == 0.0;
#else
  double c         = cosh(input);
  double s         = sinh(input);
  double t         = tanh(input);
  int    vector_ok = 1;
#endif
  return 1000 * vector_ok + 100 * (c == 1.0) + 10 * (s == 0.0) + (t == 0.0);
}

int main(void) {
  volatile int input = 7;
  printf("%d %d %d %d %d\n", cache_prefetch_probe(input), frame_probe(),
         clear_padding_probe(), frexp_probe(), hyperbolic_probe());
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(abi_unadjusted)]
// COMMON-LOWERING-NEXT: #![feature(link_llvm_intrinsics)]
// COMMON-LOWERING-NEXT: #![feature(c_variadic)]
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
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     #[link_name = "frexp"]
// COMMON-LOWERING-NEXT:     fn __slate_builtin_frexp(_0: f64, _1: *mut i32) -> f64;
// COMMON-LOWERING-NEXT:     #[link_name = "frexpf"]
// COMMON-LOWERING-NEXT:     fn __slate_builtin_frexpf(_0: f32, _1: *mut i32) -> f32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: mod __slate_bitfields {
// COMMON-LOWERING-NEXT:     #[bitfields::bitfield(
// COMMON-LOWERING-NEXT:         u8,
// COMMON-LOWERING-NEXT:         new = false,
// COMMON-LOWERING-NEXT:         from_into_bits = false,
// COMMON-LOWERING-NEXT:         from_traits = false,
// COMMON-LOWERING-NEXT:         default = false,
// COMMON-LOWERING-NEXT:         debug = false,
// COMMON-LOWERING-NEXT:         builder = false,
// COMMON-LOWERING-NEXT:         bit_ops = false
// COMMON-LOWERING-NEXT:     )]
// COMMON-LOWERING-NEXT:     pub struct __SlateBitfield_bit_padded_0 {
// COMMON-LOWERING-NEXT:         #[bits(3)]
// COMMON-LOWERING-NEXT:         pub low: u8,
// COMMON-LOWERING-NEXT:         #[bits(2)]
// COMMON-LOWERING-NEXT:         pub _reserved_0: u128,
// COMMON-LOWERING-NEXT:         #[bits(3)]
// COMMON-LOWERING-NEXT:         pub high: u8,
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct bit_padded {
// COMMON-LOWERING-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_bit_padded_0,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct padded {
// COMMON-LOWERING-NEXT:     byte: u8,
// COMMON-LOWERING-NEXT:     word: u32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut input: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = cache_prefetch_probe({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = frame_probe();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = clear_padding_probe();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = frexp_probe();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = hyperbolic_probe();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn cache_prefetch_probe({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     bytes[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         unsafe { __slate_intrinsic_clear_cache_64e8e36ba84fcffa({{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn frame_probe() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let mut {{__v[0-9]+}}: u8 = 0u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!({{__v[0-9]+}}) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn clear_padding_probe() -> i32 {
// COMMON-LOWERING-NEXT:     let mut value: padded = padded { byte: 0, word: 0 };
// COMMON-LOWERING-NEXT:     let mut bits: bit_padded = bit_padded {
// COMMON-LOWERING-NEXT:         __bitfield_0: unsafe {
// COMMON-LOWERING-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_bit_padded_0>(0)
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(value) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 255;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(bits) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 255;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 7;
// COMMON-LOWERING-NEXT:     value.byte = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 11;
// COMMON-LOWERING-NEXT:     value.word = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = ({{__v[0-9]+}} as u8) << 5 >> 5;
// COMMON-LOWERING-NEXT:     bits.__bitfield_0.set_low(({{__v[0-9]+}} as u8) << 5 >> 5);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = ({{__v[0-9]+}} as u8) << 5 >> 5;
// COMMON-LOWERING-NEXT:     bits.__bitfield_0.set_high(({{__v[0-9]+}} as u8) << 5 >> 5);
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(1usize) =
// COMMON-LOWERING-NEXT:             *(std::ptr::addr_of_mut!(value) as *mut u8).add(1usize) & 0u8;
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(2usize) =
// COMMON-LOWERING-NEXT:             *(std::ptr::addr_of_mut!(value) as *mut u8).add(2usize) & 0u8;
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(3usize) =
// COMMON-LOWERING-NEXT:             *(std::ptr::addr_of_mut!(value) as *mut u8).add(3usize) & 0u8;
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *(std::ptr::addr_of_mut!(bits) as *mut u8).add(0usize) =
// COMMON-LOWERING-NEXT:             *(std::ptr::addr_of_mut!(bits) as *mut u8).add(0usize) & 231u8;
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(value) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(bits) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 231;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn frexp_probe() -> i32 {
// COMMON-LOWERING-NEXT:     let mut input: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut input_f: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 12.0;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 8.0;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input_f), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// COMMON-LOWERING-NEXT:     let mut {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { __slate_builtin_frexp({{__v[0-9]+}}, std::ptr::addr_of_mut!({{__v[0-9]+}})) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input_f)) };
// COMMON-LOWERING-NEXT:     let mut {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { __slate_builtin_frexpf({{__v[0-9]+}}, std::ptr::addr_of_mut!({{__v[0-9]+}})) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 100;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.75;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn hyperbolic_probe() -> i32 {
// COMMON-LOWERING-NEXT:     let mut input: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut vector_input: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// COMMON-LOWERING-NEXT:     let mut vc: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// COMMON-LOWERING-NEXT:     let mut vs: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// COMMON-LOWERING-NEXT:     let mut vt: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.cosh();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.sinh();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.tanh();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [f64; 2] = [0.0, 0.0];
// COMMON-LOWERING-NEXT:     *vector_input = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [f64; 2] = [{{__v[0-9]+}}[0usize].cosh(), {{__v[0-9]+}}[1usize].cosh()];
// COMMON-LOWERING-NEXT:     *vc = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [f64; 2] = [{{__v[0-9]+}}[0usize].sinh(), {{__v[0-9]+}}[1usize].sinh()];
// COMMON-LOWERING-NEXT:     *vs = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [f64; 2] = [{{__v[0-9]+}}[0usize].tanh(), {{__v[0-9]+}}[1usize].tanh()];
// COMMON-LOWERING-NEXT:     *vt = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [f64; 2] = *vc;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: [f64; 2] = *vc;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: [f64; 2] = *vs;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: [f64; 2] = *vs;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: [f64; 2] = *vt;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: [f64; 2] = *vt;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1000;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 100;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "unadjusted" {
// COMMON-LOWERING-NEXT:     #[link_name = "llvm.clear_cache.p0"]
// COMMON-LOWERING-NEXT:     fn __slate_intrinsic_clear_cache_64e8e36ba84fcffa(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut bytes: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [0; 16];
// LOWERING-X86_64-GNU-NEXT:     *bytes = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = {{arg[0-9]+}} as i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(16) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = bytes[({{__v[0-9]+}} as usize)];
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut bytes: [u8; 16] = [0; 16];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 16] = [0; 16];
// LOWERING-AARCH64-GNU-NEXT:     bytes = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{arg[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = bytes.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = bytes.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(16) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = bytes.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = bytes[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(abi_unadjusted)]
// COMMON-REWRITES-NEXT: #![feature(link_llvm_intrinsics)]
// COMMON-REWRITES-NEXT: #![feature(c_variadic)]
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
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     #[link_name = "frexp"]
// COMMON-REWRITES-NEXT:     fn __slate_builtin_frexp(_0: f64, _1: *mut i32) -> f64;
// COMMON-REWRITES-NEXT:     #[link_name = "frexpf"]
// COMMON-REWRITES-NEXT:     fn __slate_builtin_frexpf(_0: f32, _1: *mut i32) -> f32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: mod __slate_bitfields {
// COMMON-REWRITES-NEXT:     #[bitfields::bitfield(
// COMMON-REWRITES-NEXT:         u8,
// COMMON-REWRITES-NEXT:         new = false,
// COMMON-REWRITES-NEXT:         from_into_bits = false,
// COMMON-REWRITES-NEXT:         from_traits = false,
// COMMON-REWRITES-NEXT:         default = false,
// COMMON-REWRITES-NEXT:         debug = false,
// COMMON-REWRITES-NEXT:         builder = false,
// COMMON-REWRITES-NEXT:         bit_ops = false
// COMMON-REWRITES-NEXT:     )]
// COMMON-REWRITES-NEXT:     pub struct __SlateBitfield_bit_padded_0 {
// COMMON-REWRITES-NEXT:         #[bits(3)]
// COMMON-REWRITES-NEXT:         pub low: u8,
// COMMON-REWRITES-NEXT:         #[bits(2)]
// COMMON-REWRITES-NEXT:         pub _reserved_0: u128,
// COMMON-REWRITES-NEXT:         #[bits(3)]
// COMMON-REWRITES-NEXT:         pub high: u8,
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct bit_padded {
// COMMON-REWRITES-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_bit_padded_0,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct padded {
// COMMON-REWRITES-NEXT:     byte: u8,
// COMMON-REWRITES-NEXT:     word: u32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut input: i32 = 0;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), 7 as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             cache_prefetch_probe({{__v[0-9]+}}),
// COMMON-REWRITES-NEXT:             frame_probe(),
// COMMON-REWRITES-NEXT:             clear_padding_probe(),
// COMMON-REWRITES-NEXT:             frexp_probe(),
// COMMON-REWRITES-NEXT:             hyperbolic_probe(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn cache_prefetch_probe({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             __slate_intrinsic_clear_cache_64e8e36ba84fcffa({{__v[0-9]+}}, {{__v[0-9]+}} as *mut core::ffi::c_void)
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:     (bytes[0] as i32) + 1
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn frame_probe() -> i32 {
// COMMON-REWRITES-NEXT:     let mut {{__v[0-9]+}}: u8 = 0u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!({{__v[0-9]+}}) as *mut u8;
// COMMON-REWRITES-NEXT:     (({{__v[0-9]+}} as *mut core::ffi::c_void) != std::ptr::null_mut()) as i32
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn clear_padding_probe() -> i32 {
// COMMON-REWRITES-NEXT:     let mut value: padded = padded { byte: 0, word: 0 };
// COMMON-REWRITES-NEXT:     let mut bits: bit_padded = bit_padded {
// COMMON-REWRITES-NEXT:         __bitfield_0: unsafe {
// COMMON-REWRITES-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_bit_padded_0>(0)
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(value) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{__v[0-9]+}} as *mut u8, (255 as i32) as u8, (8 as u64) as usize) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(bits) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{__v[0-9]+}} as *mut u8, (255 as i32) as u8, (1 as u64) as usize) };
// COMMON-REWRITES-NEXT:     value.byte = 7;
// COMMON-REWRITES-NEXT:     value.word = 11;
// COMMON-REWRITES-NEXT:     bits.__bitfield_0.set_low((7 as u8) << 5 >> 5);
// COMMON-REWRITES-NEXT:     bits.__bitfield_0.set_high((7 as u8) << 5 >> 5);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(1usize) =
// COMMON-REWRITES-NEXT:             *(std::ptr::addr_of_mut!(value) as *mut u8).add(1usize) & 0u8;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(2usize) =
// COMMON-REWRITES-NEXT:             *(std::ptr::addr_of_mut!(value) as *mut u8).add(2usize) & 0u8;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(3usize) =
// COMMON-REWRITES-NEXT:             *(std::ptr::addr_of_mut!(value) as *mut u8).add(3usize) & 0u8;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *(std::ptr::addr_of_mut!(bits) as *mut u8).add(0usize) =
// COMMON-REWRITES-NEXT:             *(std::ptr::addr_of_mut!(bits) as *mut u8).add(0usize) & 231u8;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(value) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *{{__v[0-9]+}} }) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ((unsafe { *{{__v[0-9]+}} }) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * ({{__v[0-9]+}} + ((unsafe { *{{__v[0-9]+}} }) as i32));
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(bits) as *mut u8;
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + ((((unsafe { *{{__v[0-9]+}} }) as i32) == 231) as i32)
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn frexp_probe() -> i32 {
// COMMON-REWRITES-NEXT:     let mut input: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut input_f: f32 = 0.0;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), 12.0 as f64) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input_f), 8.0 as f32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// COMMON-REWRITES-NEXT:     let mut {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { __slate_builtin_frexp({{__v[0-9]+}}, std::ptr::addr_of_mut!({{__v[0-9]+}})) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input_f)) };
// COMMON-REWRITES-NEXT:     let mut {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { __slate_builtin_frexpf({{__v[0-9]+}}, std::ptr::addr_of_mut!({{__v[0-9]+}})) };
// COMMON-REWRITES-NEXT:     100 * (10 * (({{__v[0-9]+}} == 0.75) as i32) + {{__v[0-9]+}}) + 10 * (({{__v[0-9]+}} == 0.5) as i32) + {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn hyperbolic_probe() -> i32 {
// COMMON-REWRITES-NEXT:     let mut input: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut vector_input: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// COMMON-REWRITES-NEXT:     let mut vc: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// COMMON-REWRITES-NEXT:     let mut vs: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// COMMON-REWRITES-NEXT:     let mut vt: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), 0.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.cosh();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.sinh();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.tanh();
// COMMON-REWRITES-NEXT:     *vector_input = [0.0, 0.0];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: [f64; 2] = [{{__v[0-9]+}}[0usize].cosh(), {{__v[0-9]+}}[1usize].cosh()];
// COMMON-REWRITES-NEXT:     *vc = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: [f64; 2] = [{{__v[0-9]+}}[0usize].sinh(), {{__v[0-9]+}}[1usize].sinh()];
// COMMON-REWRITES-NEXT:     *vs = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: [f64; 2] = [{{__v[0-9]+}}[0usize].tanh(), {{__v[0-9]+}}[1usize].tanh()];
// COMMON-REWRITES-NEXT:     *vt = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if (*vc)[0] == 1.0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (*vc)[1] == 1.0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (*vs)[0] == 0.0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (*vs)[1] == 0.0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (*vt)[0] == 0.0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (*vt)[1] == 0.0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     1000 * ({{__v[0-9]+}} as i32)
// COMMON-REWRITES-NEXT:         + 100 * (({{__v[0-9]+}} == 1.0) as i32)
// COMMON-REWRITES-NEXT:         + 10 * (({{__v[0-9]+}} == 0.0) as i32)
// COMMON-REWRITES-NEXT:         + (({{__v[0-9]+}} == 0.0) as i32)
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "unadjusted" {
// COMMON-REWRITES-NEXT:     #[link_name = "llvm.clear_cache.p0"]
// COMMON-REWRITES-NEXT:     fn __slate_intrinsic_clear_cache_64e8e36ba84fcffa(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %d %d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let mut bytes: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-X86_64-GNU-NEXT:     *bytes = [0; 16];
// REWRITES-X86_64-GNU-NEXT:     bytes[({{__v[0-9]+}} as usize)] = {{arg[0-9]+}} as i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(16) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d %d %d\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut bytes: [u8; 16] = [0; 16];
// REWRITES-AARCH64-GNU-NEXT:     bytes[({{__v[0-9]+}} as usize)] = {{arg[0-9]+}} as u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = bytes.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = bytes.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(16) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = bytes.as_mut_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
