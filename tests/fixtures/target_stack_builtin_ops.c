#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct padded {
  unsigned char byte;
  unsigned int  word;
};

struct bit_padded {
  unsigned char low : 3;
  unsigned char : 2;
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
  struct padded value;
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
  unsigned char *padding = (unsigned char *)&value;
  padding[1]             = 0;
  padding[2]             = 0;
  padding[3]             = 0;
  *(unsigned char *)&bits &= 0xe7;
#endif
  unsigned char *bytes = (unsigned char *)&value;
  return 10 * (bytes[1] + bytes[2] + bytes[3]) +
         (*(unsigned char *)&bits == 0xe7);
}

static int frexp_probe(void) {
  volatile double input = 12.0;
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
  double          c     = __builtin_elementwise_cosh(input);
  double          s     = __builtin_elementwise_sinh(input);
  double          t     = __builtin_elementwise_tanh(input);
  volatile double2 vector_input = {0.0, 0.0};
  double2          vc = __builtin_elementwise_cosh(vector_input);
  double2          vs = __builtin_elementwise_sinh(vector_input);
  double2          vt = __builtin_elementwise_tanh(vector_input);
  int vector_ok = vc[0] == 1.0 && vc[1] == 1.0 && vs[0] == 0.0 &&
                  vs[1] == 0.0 && vt[0] == 0.0 && vt[1] == 0.0;
#else
  double c = cosh(input);
  double s = sinh(input);
  double t = tanh(input);
  int    vector_ok = 1;
#endif
  return 1000 * vector_ok + 100 * (c == 1.0) + 10 * (s == 0.0) +
         (t == 0.0);
}

int main(void) {
  volatile int input = 7;
  printf("%d %d %d %d %d\n", cache_prefetch_probe(input), frame_probe(),
         clear_padding_probe(), frexp_probe(), hyperbolic_probe());
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(abi_unadjusted)]
// LOWERING-NEXT: #![feature(link_llvm_intrinsics)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     #[link_name = "frexp"]
// LOWERING-NEXT:     fn __slate_builtin_frexp(_0: f64, _1: *mut i32) -> f64;
// LOWERING-NEXT:     #[link_name = "frexpf"]
// LOWERING-NEXT:     fn __slate_builtin_frexpf(_0: f32, _1: *mut i32) -> f32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: mod __slate_bitfields {
// LOWERING-NEXT:     #[bitfields::bitfield(u8, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// LOWERING-NEXT:     pub struct __SlateBitfield_bit_padded_0 {
// LOWERING-NEXT:         #[bits(3)]
// LOWERING-NEXT:         pub low: u8,
// LOWERING-NEXT:         #[bits(2)]
// LOWERING-NEXT:         pub _reserved_0: u128,
// LOWERING-NEXT:         #[bits(3)]
// LOWERING-NEXT:         pub high: u8,
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct bit_padded {
// LOWERING-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_bit_padded_0,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct padded {
// LOWERING-NEXT:     byte: u8,
// LOWERING-NEXT:     word: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn cache_prefetch_probe({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut bytes: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     *bytes = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = {{arg[0-9]+}} as i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     bytes[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(16) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         unsafe { __slate_intrinsic_clear_cache_64e8e36ba84fcffa({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = bytes[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn frame_probe() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let mut {{_v[0-9]+}}: u8 = 0u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!({{_v[0-9]+}}) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn clear_padding_probe() -> i32 {
// LOWERING-NEXT:     let mut value: padded = padded { byte: 0, word: 0 };
// LOWERING-NEXT:     let mut bits: bit_padded = bit_padded { __bitfield_0: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_bit_padded_0>(0) } };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(value) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 255;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(bits) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 255;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 7;
// LOWERING-NEXT:     value.byte = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 11;
// LOWERING-NEXT:     value.word = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = ({{_v[0-9]+}} as u8) << 5 >> 5;
// LOWERING-NEXT:     bits.__bitfield_0.set_low(({{_v[0-9]+}} as u8) << 5 >> 5);
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = ({{_v[0-9]+}} as u8) << 5 >> 5;
// LOWERING-NEXT:     bits.__bitfield_0.set_high(({{_v[0-9]+}} as u8) << 5 >> 5);
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(1usize) = *(std::ptr::addr_of_mut!(value) as *mut u8).add(1usize) & 0u8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(2usize) = *(std::ptr::addr_of_mut!(value) as *mut u8).add(2usize) & 0u8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(3usize) = *(std::ptr::addr_of_mut!(value) as *mut u8).add(3usize) & 0u8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *(std::ptr::addr_of_mut!(bits) as *mut u8).add(0usize) = *(std::ptr::addr_of_mut!(bits) as *mut u8).add(0usize) & 231u8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(value) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(2) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(3) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(bits) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 231;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn frexp_probe() -> i32 {
// LOWERING-NEXT:     let mut input: f64 = 0.0;
// LOWERING-NEXT:     let mut input_f: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 12.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 8.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input_f), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let mut {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { __slate_builtin_frexp({{_v[0-9]+}}, std::ptr::addr_of_mut!({{_v[0-9]+}})) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input_f)) };
// LOWERING-NEXT:     let mut {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { __slate_builtin_frexpf({{_v[0-9]+}}, std::ptr::addr_of_mut!({{_v[0-9]+}})) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.75;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn hyperbolic_probe() -> i32 {
// LOWERING-NEXT:     let mut input: f64 = 0.0;
// LOWERING-NEXT:     let mut vector_input: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// LOWERING-NEXT:     let mut vc: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// LOWERING-NEXT:     let mut vs: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// LOWERING-NEXT:     let mut vt: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.cosh();
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.sinh();
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.tanh();
// LOWERING-NEXT:     let {{_v[0-9]+}}: [f64; 2] = [0.0, 0.0];
// LOWERING-NEXT:     *vector_input = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: [f64; 2] = [{{_v[0-9]+}}[0usize].cosh(), {{_v[0-9]+}}[1usize].cosh()];
// LOWERING-NEXT:     *vc = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: [f64; 2] = [{{_v[0-9]+}}[0usize].sinh(), {{_v[0-9]+}}[1usize].sinh()];
// LOWERING-NEXT:     *vs = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: [f64; 2] = [{{_v[0-9]+}}[0usize].tanh(), {{_v[0-9]+}}[1usize].tanh()];
// LOWERING-NEXT:     *vt = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: [f64; 2] = *vc;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: [f64; 2] = *vc;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: [f64; 2] = *vs;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: [f64; 2] = *vs;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: [f64; 2] = *vt;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: [f64; 2] = *vt;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1000;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut input: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = cache_prefetch_probe({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = frame_probe();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = clear_padding_probe();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = frexp_probe();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = hyperbolic_probe();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "unadjusted" {
// LOWERING-NEXT:     #[link_name = "llvm.clear_cache"]
// LOWERING-NEXT:     fn __slate_intrinsic_clear_cache_64e8e36ba84fcffa(_0: *mut core::ffi::c_void, _1: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(abi_unadjusted)]
// REWRITES-NEXT: #![feature(link_llvm_intrinsics)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     #[link_name = "frexp"]
// REWRITES-NEXT:     fn __slate_builtin_frexp(_0: f64, _1: *mut i32) -> f64;
// REWRITES-NEXT:     #[link_name = "frexpf"]
// REWRITES-NEXT:     fn __slate_builtin_frexpf(_0: f32, _1: *mut i32) -> f32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: mod __slate_bitfields {
// REWRITES-NEXT:     #[bitfields::bitfield(u8, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// REWRITES-NEXT:     pub struct __SlateBitfield_bit_padded_0 {
// REWRITES-NEXT:         #[bits(3)]
// REWRITES-NEXT:         pub low: u8,
// REWRITES-NEXT:         #[bits(2)]
// REWRITES-NEXT:         pub _reserved_0: u128,
// REWRITES-NEXT:         #[bits(3)]
// REWRITES-NEXT:         pub high: u8,
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct bit_padded {
// REWRITES-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_bit_padded_0,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct padded {
// REWRITES-NEXT:     byte: u8,
// REWRITES-NEXT:     word: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn cache_prefetch_probe({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut bytes: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: *bytes = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: bytes[({{_v[0-9]+}} as usize)] = {{arg[0-9]+}} as i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(16) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         unsafe { __slate_intrinsic_clear_cache_64e8e36ba84fcffa({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: return (bytes[({{_v[0-9]+}} as usize)] as i32) + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn frame_probe() -> i32 {
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 0;
// REWRITES-NEXT: let mut {{_v[0-9]+}}: u8 = 0u8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!({{_v[0-9]+}}) as *mut u8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: return (({{_v[0-9]+}} as *mut core::ffi::c_void) != {{_v[0-9]+}}) as i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn clear_padding_probe() -> i32 {
// REWRITES-NEXT: let mut value: padded = padded { byte: 0, word: 0 };
// REWRITES-NEXT: let mut bits: bit_padded = bit_padded { __bitfield_0: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_bit_padded_0>(0) } };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(value) as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 255;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 8;
// REWRITES-NEXT: unsafe { std::ptr::write_bytes(({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut u8, ({{_v[0-9]+}} as i32) as u8, ({{_v[0-9]+}} as usize) as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(bits) as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 255;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: unsafe { std::ptr::write_bytes(({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut u8, ({{_v[0-9]+}} as i32) as u8, ({{_v[0-9]+}} as usize) as usize) };
// REWRITES-NEXT: value.byte = 7;
// REWRITES-NEXT: value.word = 11;
// REWRITES-NEXT: let {{_v[0-9]+}}: u8 = 7;
// REWRITES-NEXT: let {{_v[0-9]+}}: u8 = ({{_v[0-9]+}} as u8) << 5 >> 5;
// REWRITES-NEXT: bits.__bitfield_0.set_low(({{_v[0-9]+}} as u8) << 5 >> 5);
// REWRITES-NEXT: let {{_v[0-9]+}}: u8 = 7;
// REWRITES-NEXT: let {{_v[0-9]+}}: u8 = ({{_v[0-9]+}} as u8) << 5 >> 5;
// REWRITES-NEXT: bits.__bitfield_0.set_high(({{_v[0-9]+}} as u8) << 5 >> 5);
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(1usize) = *(std::ptr::addr_of_mut!(value) as *mut u8).add(1usize) & 0u8;
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(2usize) = *(std::ptr::addr_of_mut!(value) as *mut u8).add(2usize) & 0u8;
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *(std::ptr::addr_of_mut!(value) as *mut u8).add(3usize) = *(std::ptr::addr_of_mut!(value) as *mut u8).add(3usize) & 0u8;
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *(std::ptr::addr_of_mut!(bits) as *mut u8).add(0usize) = *(std::ptr::addr_of_mut!(bits) as *mut u8).add(0usize) & 231u8;
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(value) as *mut u8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = (unsafe { *{{_v[0-9]+}} }) as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(2) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ((unsafe { *{{_v[0-9]+}} }) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(3) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * ({{_v[0-9]+}} + ((unsafe { *{{_v[0-9]+}} }) as i32));
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(bits) as *mut u8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 231;
// REWRITES-NEXT: return {{_v[0-9]+}} + ((((unsafe { *{{_v[0-9]+}} }) as i32) == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn frexp_probe() -> i32 {
// REWRITES-NEXT: let mut input: f64 = 0.0;
// REWRITES-NEXT: let mut input_f: f32 = 0.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 12.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = 8.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input_f), {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: let mut {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { __slate_builtin_frexp({{_v[0-9]+}}, std::ptr::addr_of_mut!({{_v[0-9]+}})) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input_f)) };
// REWRITES-NEXT: let mut {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = unsafe { __slate_builtin_frexpf({{_v[0-9]+}}, std::ptr::addr_of_mut!({{_v[0-9]+}})) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 100;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 0.75;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = 0.5;
// REWRITES-NEXT: return {{_v[0-9]+}} * ({{_v[0-9]+}} * (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + {{_v[0-9]+}}) + {{_v[0-9]+}} * (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn hyperbolic_probe() -> i32 {
// REWRITES-NEXT: let mut input: f64 = 0.0;
// REWRITES-NEXT: let mut vector_input: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// REWRITES-NEXT: let mut vc: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// REWRITES-NEXT: let mut vs: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// REWRITES-NEXT: let mut vt: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 0.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.cosh();
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.sinh();
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.tanh();
// REWRITES-NEXT: *vector_input = [0.0, 0.0];
// REWRITES-NEXT: let {{_v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: [f64; 2] = [{{_v[0-9]+}}[0usize].cosh(), {{_v[0-9]+}}[1usize].cosh()];
// REWRITES-NEXT: *vc = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: [f64; 2] = [{{_v[0-9]+}}[0usize].sinh(), {{_v[0-9]+}}[1usize].sinh()];
// REWRITES-NEXT: *vs = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: [f64; 2] = [{{_v[0-9]+}}[0usize].tanh(), {{_v[0-9]+}}[1usize].tanh()];
// REWRITES-NEXT: *vt = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 1.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if (*vc)[({{_v[0-9]+}} as usize)] == {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:         let {{_v[0-9]+}}: f64 = 1.0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (*vc)[({{_v[0-9]+}} as usize)] == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: f64 = 0.0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (*vs)[({{_v[0-9]+}} as usize)] == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:         let {{_v[0-9]+}}: f64 = 0.0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (*vs)[({{_v[0-9]+}} as usize)] == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: f64 = 0.0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (*vt)[({{_v[0-9]+}} as usize)] == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:         let {{_v[0-9]+}}: f64 = 0.0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (*vt)[({{_v[0-9]+}} as usize)] == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1000;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 100;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 1.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 0.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 0.0;
// REWRITES-NEXT: return {{_v[0-9]+}} * ({{_v[0-9]+}} as i32) + {{_v[0-9]+}} * (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + {{_v[0-9]+}} * (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut input: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 7;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = cache_prefetch_probe({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = frame_probe();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = clear_padding_probe();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = frexp_probe();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = hyperbolic_probe();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "unadjusted" {
// REWRITES-NEXT:     #[link_name = "llvm.clear_cache"]
// REWRITES-NEXT:     fn __slate_intrinsic_clear_cache_64e8e36ba84fcffa(_0: *mut core::ffi::c_void, _1: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
