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
// LOWERING-NEXT: fn cache_prefetch_probe(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut bytes: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     x = arg0;
// LOWERING-NEXT:     *bytes = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i8 = _v0 as i8;
// LOWERING-NEXT:     let _v2: i64 = 0;
// LOWERING-NEXT:     bytes[(_v2 as usize)] = _v1;
// LOWERING-NEXT:     let _v3: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = _v3 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v5: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: u64 = 16;
// LOWERING-NEXT:     let _v7: *mut i8 = unsafe { _v5.add(16) };
// LOWERING-NEXT:     let _v8: *mut core::ffi::c_void = _v7 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         unsafe { __slate_intrinsic_clear_cache_64e8e36ba84fcffa(_v4, _v8) };
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v9: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: i32 = 1;
// LOWERING-NEXT:     let _v11: *mut i8 = unsafe { _v9.add(1) };
// LOWERING-NEXT:     let _v12: *mut core::ffi::c_void = _v11 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v13: i64 = 0;
// LOWERING-NEXT:     let _v14: i8 = bytes[(_v13 as usize)];
// LOWERING-NEXT:     let _v15: i32 = _v14 as i32;
// LOWERING-NEXT:     let _v16: i32 = 1;
// LOWERING-NEXT:     let _v17: i32 = _v15 + _v16;
// LOWERING-NEXT:     __retval = _v17;
// LOWERING-NEXT:     let _v18: i32 = __retval;
// LOWERING-NEXT:     return _v18;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn frame_probe() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut frame: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: u32 = 0;
// LOWERING-NEXT:     let mut _v1: u8 = 0u8;
// LOWERING-NEXT:     let _v2: *mut u8 = std::ptr::addr_of_mut!(_v1) as *mut u8;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = _v2 as *mut core::ffi::c_void;
// LOWERING-NEXT:     frame = _v3;
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = frame;
// LOWERING-NEXT:     let _v5: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v6: bool = _v4 != _v5;
// LOWERING-NEXT:     let _v7: i32 = _v6 as i32;
// LOWERING-NEXT:     __retval = _v7;
// LOWERING-NEXT:     let _v8: i32 = __retval;
// LOWERING-NEXT:     return _v8;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn clear_padding_probe() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut value: padded = padded { byte: 0, word: 0 };
// LOWERING-NEXT:     let mut bits: bit_padded = bit_padded { __bitfield_0: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_bit_padded_0>(0) } };
// LOWERING-NEXT:     let mut bytes: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = std::ptr::addr_of_mut!(value) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v1: i32 = 255;
// LOWERING-NEXT:     let _v2: u64 = 8;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = unsafe { memset(_v0 as *mut core::ffi::c_void, _v1 as i32, _v2 as usize) };
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = std::ptr::addr_of_mut!(bits) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v5: i32 = 255;
// LOWERING-NEXT:     let _v6: u64 = 1;
// LOWERING-NEXT:     let _v7: *mut core::ffi::c_void = unsafe { memset(_v4 as *mut core::ffi::c_void, _v5 as i32, _v6 as usize) };
// LOWERING-NEXT:     let _v8: u8 = 7;
// LOWERING-NEXT:     value.byte = _v8;
// LOWERING-NEXT:     let _v9: u32 = 11;
// LOWERING-NEXT:     value.word = _v9;
// LOWERING-NEXT:     let _v10: u8 = 7;
// LOWERING-NEXT:     let _v11: u8 = (_v10 as u8) << 5 >> 5;
// LOWERING-NEXT:     bits.__bitfield_0.set_low((_v10 as u8) << 5 >> 5);
// LOWERING-NEXT:     let _v12: u8 = 7;
// LOWERING-NEXT:     let _v13: u8 = (_v12 as u8) << 5 >> 5;
// LOWERING-NEXT:     bits.__bitfield_0.set_high((_v12 as u8) << 5 >> 5);
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
// LOWERING-NEXT:     let _v14: *mut u8 = std::ptr::addr_of_mut!(value) as *mut u8;
// LOWERING-NEXT:     bytes = _v14;
// LOWERING-NEXT:     let _v15: i32 = 10;
// LOWERING-NEXT:     let _v16: i64 = 1;
// LOWERING-NEXT:     let _v17: *mut u8 = bytes;
// LOWERING-NEXT:     let _v18: *mut u8 = unsafe { _v17.add(1) };
// LOWERING-NEXT:     let _v19: u8 = unsafe { *_v18 };
// LOWERING-NEXT:     let _v20: i32 = _v19 as i32;
// LOWERING-NEXT:     let _v21: i64 = 2;
// LOWERING-NEXT:     let _v22: *mut u8 = bytes;
// LOWERING-NEXT:     let _v23: *mut u8 = unsafe { _v22.add(2) };
// LOWERING-NEXT:     let _v24: u8 = unsafe { *_v23 };
// LOWERING-NEXT:     let _v25: i32 = _v24 as i32;
// LOWERING-NEXT:     let _v26: i32 = _v20 + _v25;
// LOWERING-NEXT:     let _v27: i64 = 3;
// LOWERING-NEXT:     let _v28: *mut u8 = bytes;
// LOWERING-NEXT:     let _v29: *mut u8 = unsafe { _v28.add(3) };
// LOWERING-NEXT:     let _v30: u8 = unsafe { *_v29 };
// LOWERING-NEXT:     let _v31: i32 = _v30 as i32;
// LOWERING-NEXT:     let _v32: i32 = _v26 + _v31;
// LOWERING-NEXT:     let _v33: i32 = _v15 * _v32;
// LOWERING-NEXT:     let _v34: *mut u8 = std::ptr::addr_of_mut!(bits) as *mut u8;
// LOWERING-NEXT:     let _v35: u8 = unsafe { *_v34 };
// LOWERING-NEXT:     let _v36: i32 = _v35 as i32;
// LOWERING-NEXT:     let _v37: i32 = 231;
// LOWERING-NEXT:     let _v38: bool = _v36 == _v37;
// LOWERING-NEXT:     let _v39: i32 = _v38 as i32;
// LOWERING-NEXT:     let _v40: i32 = _v33 + _v39;
// LOWERING-NEXT:     __retval = _v40;
// LOWERING-NEXT:     let _v41: i32 = __retval;
// LOWERING-NEXT:     return _v41;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn frexp_probe() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut input: f64 = 0.0;
// LOWERING-NEXT:     let mut input_f: f32 = 0.0;
// LOWERING-NEXT:     let mut exponent_d: i32 = 0;
// LOWERING-NEXT:     let mut exponent_f: i32 = 0;
// LOWERING-NEXT:     let mut fraction_d: f64 = 0.0;
// LOWERING-NEXT:     let mut fraction_f: f32 = 0.0;
// LOWERING-NEXT:     let _v0: f64 = 12.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), _v0) };
// LOWERING-NEXT:     let _v1: f32 = 8.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input_f), _v1) };
// LOWERING-NEXT:     let _v2: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let mut _v3: i32 = 0;
// LOWERING-NEXT:     let _v4: f64 = unsafe { __slate_builtin_frexp(_v2, std::ptr::addr_of_mut!(_v3)) };
// LOWERING-NEXT:     let _v5: i32 = _v3;
// LOWERING-NEXT:     exponent_d = _v5;
// LOWERING-NEXT:     fraction_d = _v4;
// LOWERING-NEXT:     let _v6: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input_f)) };
// LOWERING-NEXT:     let mut _v7: i32 = 0;
// LOWERING-NEXT:     let _v8: f32 = unsafe { __slate_builtin_frexpf(_v6, std::ptr::addr_of_mut!(_v7)) };
// LOWERING-NEXT:     let _v9: i32 = _v7;
// LOWERING-NEXT:     exponent_f = _v9;
// LOWERING-NEXT:     fraction_f = _v8;
// LOWERING-NEXT:     let _v10: i32 = 100;
// LOWERING-NEXT:     let _v11: i32 = 10;
// LOWERING-NEXT:     let _v12: f64 = fraction_d;
// LOWERING-NEXT:     let _v13: f64 = 0.75;
// LOWERING-NEXT:     let _v14: bool = _v12 == _v13;
// LOWERING-NEXT:     let _v15: i32 = _v14 as i32;
// LOWERING-NEXT:     let _v16: i32 = _v11 * _v15;
// LOWERING-NEXT:     let _v17: i32 = exponent_d;
// LOWERING-NEXT:     let _v18: i32 = _v16 + _v17;
// LOWERING-NEXT:     let _v19: i32 = _v10 * _v18;
// LOWERING-NEXT:     let _v20: i32 = 10;
// LOWERING-NEXT:     let _v21: f32 = fraction_f;
// LOWERING-NEXT:     let _v22: f32 = 0.5;
// LOWERING-NEXT:     let _v23: bool = _v21 == _v22;
// LOWERING-NEXT:     let _v24: i32 = _v23 as i32;
// LOWERING-NEXT:     let _v25: i32 = _v20 * _v24;
// LOWERING-NEXT:     let _v26: i32 = _v19 + _v25;
// LOWERING-NEXT:     let _v27: i32 = exponent_f;
// LOWERING-NEXT:     let _v28: i32 = _v26 + _v27;
// LOWERING-NEXT:     __retval = _v28;
// LOWERING-NEXT:     let _v29: i32 = __retval;
// LOWERING-NEXT:     return _v29;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn hyperbolic_probe() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut input: f64 = 0.0;
// LOWERING-NEXT:     let mut c: f64 = 0.0;
// LOWERING-NEXT:     let mut s: f64 = 0.0;
// LOWERING-NEXT:     let mut t: f64 = 0.0;
// LOWERING-NEXT:     let mut vector_input: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// LOWERING-NEXT:     let mut vc: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// LOWERING-NEXT:     let mut vs: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// LOWERING-NEXT:     let mut vt: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// LOWERING-NEXT:     let mut vector_ok: i32 = 0;
// LOWERING-NEXT:     let _v0: f64 = 0.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), _v0) };
// LOWERING-NEXT:     let _v1: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let _v2: f64 = _v1.cosh();
// LOWERING-NEXT:     c = _v2;
// LOWERING-NEXT:     let _v3: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let _v4: f64 = _v3.sinh();
// LOWERING-NEXT:     s = _v4;
// LOWERING-NEXT:     let _v5: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let _v6: f64 = _v5.tanh();
// LOWERING-NEXT:     t = _v6;
// LOWERING-NEXT:     let _v7: [f64; 2] = [0.0, 0.0];
// LOWERING-NEXT:     *vector_input = _v7;
// LOWERING-NEXT:     let _v8: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// LOWERING-NEXT:     let _v9: [f64; 2] = [_v8[0usize].cosh(), _v8[1usize].cosh()];
// LOWERING-NEXT:     *vc = _v9;
// LOWERING-NEXT:     let _v10: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// LOWERING-NEXT:     let _v11: [f64; 2] = [_v10[0usize].sinh(), _v10[1usize].sinh()];
// LOWERING-NEXT:     *vs = _v11;
// LOWERING-NEXT:     let _v12: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// LOWERING-NEXT:     let _v13: [f64; 2] = [_v12[0usize].tanh(), _v12[1usize].tanh()];
// LOWERING-NEXT:     *vt = _v13;
// LOWERING-NEXT:     let _v14: [f64; 2] = *vc;
// LOWERING-NEXT:     let _v15: i32 = 0;
// LOWERING-NEXT:     let _v16: f64 = _v14[(_v15 as usize)];
// LOWERING-NEXT:     let _v17: f64 = 1.0;
// LOWERING-NEXT:     let _v18: bool = _v16 == _v17;
// LOWERING-NEXT:     let _v19: bool = if _v18 {
// LOWERING-NEXT:         let _v20: [f64; 2] = *vc;
// LOWERING-NEXT:         let _v21: i32 = 1;
// LOWERING-NEXT:         let _v22: f64 = _v20[(_v21 as usize)];
// LOWERING-NEXT:         let _v23: f64 = 1.0;
// LOWERING-NEXT:         let _v24: bool = _v22 == _v23;
// LOWERING-NEXT:         _v24
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v25: bool = false;
// LOWERING-NEXT:         _v25
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v26: bool = if _v19 {
// LOWERING-NEXT:         let _v27: [f64; 2] = *vs;
// LOWERING-NEXT:         let _v28: i32 = 0;
// LOWERING-NEXT:         let _v29: f64 = _v27[(_v28 as usize)];
// LOWERING-NEXT:         let _v30: f64 = 0.0;
// LOWERING-NEXT:         let _v31: bool = _v29 == _v30;
// LOWERING-NEXT:         _v31
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v32: bool = false;
// LOWERING-NEXT:         _v32
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v33: bool = if _v26 {
// LOWERING-NEXT:         let _v34: [f64; 2] = *vs;
// LOWERING-NEXT:         let _v35: i32 = 1;
// LOWERING-NEXT:         let _v36: f64 = _v34[(_v35 as usize)];
// LOWERING-NEXT:         let _v37: f64 = 0.0;
// LOWERING-NEXT:         let _v38: bool = _v36 == _v37;
// LOWERING-NEXT:         _v38
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v39: bool = false;
// LOWERING-NEXT:         _v39
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v40: bool = if _v33 {
// LOWERING-NEXT:         let _v41: [f64; 2] = *vt;
// LOWERING-NEXT:         let _v42: i32 = 0;
// LOWERING-NEXT:         let _v43: f64 = _v41[(_v42 as usize)];
// LOWERING-NEXT:         let _v44: f64 = 0.0;
// LOWERING-NEXT:         let _v45: bool = _v43 == _v44;
// LOWERING-NEXT:         _v45
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v46: bool = false;
// LOWERING-NEXT:         _v46
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v47: bool = if _v40 {
// LOWERING-NEXT:         let _v48: [f64; 2] = *vt;
// LOWERING-NEXT:         let _v49: i32 = 1;
// LOWERING-NEXT:         let _v50: f64 = _v48[(_v49 as usize)];
// LOWERING-NEXT:         let _v51: f64 = 0.0;
// LOWERING-NEXT:         let _v52: bool = _v50 == _v51;
// LOWERING-NEXT:         _v52
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v53: bool = false;
// LOWERING-NEXT:         _v53
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v54: i32 = _v47 as i32;
// LOWERING-NEXT:     vector_ok = _v54;
// LOWERING-NEXT:     let _v55: i32 = 1000;
// LOWERING-NEXT:     let _v56: i32 = vector_ok;
// LOWERING-NEXT:     let _v57: i32 = _v55 * _v56;
// LOWERING-NEXT:     let _v58: i32 = 100;
// LOWERING-NEXT:     let _v59: f64 = c;
// LOWERING-NEXT:     let _v60: f64 = 1.0;
// LOWERING-NEXT:     let _v61: bool = _v59 == _v60;
// LOWERING-NEXT:     let _v62: i32 = _v61 as i32;
// LOWERING-NEXT:     let _v63: i32 = _v58 * _v62;
// LOWERING-NEXT:     let _v64: i32 = _v57 + _v63;
// LOWERING-NEXT:     let _v65: i32 = 10;
// LOWERING-NEXT:     let _v66: f64 = s;
// LOWERING-NEXT:     let _v67: f64 = 0.0;
// LOWERING-NEXT:     let _v68: bool = _v66 == _v67;
// LOWERING-NEXT:     let _v69: i32 = _v68 as i32;
// LOWERING-NEXT:     let _v70: i32 = _v65 * _v69;
// LOWERING-NEXT:     let _v71: i32 = _v64 + _v70;
// LOWERING-NEXT:     let _v72: f64 = t;
// LOWERING-NEXT:     let _v73: f64 = 0.0;
// LOWERING-NEXT:     let _v74: bool = _v72 == _v73;
// LOWERING-NEXT:     let _v75: i32 = _v74 as i32;
// LOWERING-NEXT:     let _v76: i32 = _v71 + _v75;
// LOWERING-NEXT:     __retval = _v76;
// LOWERING-NEXT:     let _v77: i32 = __retval;
// LOWERING-NEXT:     return _v77;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut input: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 7;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), _v1) };
// LOWERING-NEXT:     let _v2: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let _v4: i32 = cache_prefetch_probe(_v3);
// LOWERING-NEXT:     let _v5: i32 = frame_probe();
// LOWERING-NEXT:     let _v6: i32 = clear_padding_probe();
// LOWERING-NEXT:     let _v7: i32 = frexp_probe();
// LOWERING-NEXT:     let _v8: i32 = hyperbolic_probe();
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v2 as *const i8, _v4, _v5, _v6, _v7, _v8) };
// LOWERING-NEXT:     let _v10: i32 = 0;
// LOWERING-NEXT:     __retval = _v10;
// LOWERING-NEXT:     let _v11: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v11 as i32);
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
// REWRITES-NEXT: fn cache_prefetch_probe(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut bytes: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: *bytes = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: let _v2: i64 = 0;
// REWRITES-NEXT: bytes[(_v2 as usize)] = x as i8;
// REWRITES-NEXT: let _v3: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: *mut core::ffi::c_void = _v3 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v5: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: u64 = 16;
// REWRITES-NEXT: let _v7: *mut i8 = unsafe { _v5.add(16) };
// REWRITES-NEXT: let _v8: *mut core::ffi::c_void = _v7 as *mut core::ffi::c_void;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         unsafe { __slate_intrinsic_clear_cache_64e8e36ba84fcffa(_v4, _v8) };
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v9: *mut i8 = bytes.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: i32 = 1;
// REWRITES-NEXT: let _v11: *mut i8 = unsafe { _v9.add(1) };
// REWRITES-NEXT: let _v12: *mut core::ffi::c_void = _v11 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v13: i64 = 0;
// REWRITES-NEXT: let _v16: i32 = 1;
// REWRITES-NEXT: __retval = (bytes[(_v13 as usize)] as i32) + _v16;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn frame_probe() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut frame: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v0: u32 = 0;
// REWRITES-NEXT: let mut _v1: u8 = 0u8;
// REWRITES-NEXT: let _v2: *mut u8 = std::ptr::addr_of_mut!(_v1) as *mut u8;
// REWRITES-NEXT: frame = _v2 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v5: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: __retval = (frame != _v5) as i32;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn clear_padding_probe() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut value: padded = padded { byte: 0, word: 0 };
// REWRITES-NEXT: let mut bits: bit_padded = bit_padded { __bitfield_0: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_bit_padded_0>(0) } };
// REWRITES-NEXT: let mut bytes: *mut u8 = std::ptr::null_mut();
// REWRITES-NEXT: let _v0: *mut core::ffi::c_void = std::ptr::addr_of_mut!(value) as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v1: i32 = 255;
// REWRITES-NEXT: let _v2: u64 = 8;
// REWRITES-NEXT: unsafe { std::ptr::write_bytes((_v0 as *mut core::ffi::c_void) as *mut u8, (_v1 as i32) as u8, (_v2 as usize) as usize) };
// REWRITES-NEXT: let _v4: *mut core::ffi::c_void = std::ptr::addr_of_mut!(bits) as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v5: i32 = 255;
// REWRITES-NEXT: let _v6: u64 = 1;
// REWRITES-NEXT: unsafe { std::ptr::write_bytes((_v4 as *mut core::ffi::c_void) as *mut u8, (_v5 as i32) as u8, (_v6 as usize) as usize) };
// REWRITES-NEXT: value.byte = 7;
// REWRITES-NEXT: value.word = 11;
// REWRITES-NEXT: let _v10: u8 = 7;
// REWRITES-NEXT: let _v11: u8 = (_v10 as u8) << 5 >> 5;
// REWRITES-NEXT: bits.__bitfield_0.set_low((_v10 as u8) << 5 >> 5);
// REWRITES-NEXT: let _v12: u8 = 7;
// REWRITES-NEXT: let _v13: u8 = (_v12 as u8) << 5 >> 5;
// REWRITES-NEXT: bits.__bitfield_0.set_high((_v12 as u8) << 5 >> 5);
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
// REWRITES-NEXT: let _v14: *mut u8 = std::ptr::addr_of_mut!(value) as *mut u8;
// REWRITES-NEXT: bytes = _v14;
// REWRITES-NEXT: let _v15: i32 = 10;
// REWRITES-NEXT: let _v16: i64 = 1;
// REWRITES-NEXT: let _v17: *mut u8 = bytes;
// REWRITES-NEXT: let _v18: *mut u8 = unsafe { _v17.add(1) };
// REWRITES-NEXT: let _v20: i32 = (unsafe { *_v18 }) as i32;
// REWRITES-NEXT: let _v21: i64 = 2;
// REWRITES-NEXT: let _v22: *mut u8 = bytes;
// REWRITES-NEXT: let _v23: *mut u8 = unsafe { _v22.add(2) };
// REWRITES-NEXT: let _v26: i32 = _v20 + ((unsafe { *_v23 }) as i32);
// REWRITES-NEXT: let _v27: i64 = 3;
// REWRITES-NEXT: let _v28: *mut u8 = bytes;
// REWRITES-NEXT: let _v29: *mut u8 = unsafe { _v28.add(3) };
// REWRITES-NEXT: let _v33: i32 = _v15 * (_v26 + ((unsafe { *_v29 }) as i32));
// REWRITES-NEXT: let _v34: *mut u8 = std::ptr::addr_of_mut!(bits) as *mut u8;
// REWRITES-NEXT: let _v37: i32 = 231;
// REWRITES-NEXT: __retval = _v33 + ((((unsafe { *_v34 }) as i32) == _v37) as i32);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn frexp_probe() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut input: f64 = 0.0;
// REWRITES-NEXT: let mut input_f: f32 = 0.0;
// REWRITES-NEXT: let mut exponent_d: i32 = 0;
// REWRITES-NEXT: let mut exponent_f: i32 = 0;
// REWRITES-NEXT: let mut fraction_d: f64 = 0.0;
// REWRITES-NEXT: let mut fraction_f: f32 = 0.0;
// REWRITES-NEXT: let _v0: f64 = 12.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), _v0) };
// REWRITES-NEXT: let _v1: f32 = 8.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input_f), _v1) };
// REWRITES-NEXT: let _v2: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: let mut _v3: i32 = 0;
// REWRITES-NEXT: let _v4: f64 = unsafe { __slate_builtin_frexp(_v2, std::ptr::addr_of_mut!(_v3)) };
// REWRITES-NEXT: exponent_d = _v3;
// REWRITES-NEXT: fraction_d = _v4;
// REWRITES-NEXT: let _v6: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input_f)) };
// REWRITES-NEXT: let mut _v7: i32 = 0;
// REWRITES-NEXT: let _v8: f32 = unsafe { __slate_builtin_frexpf(_v6, std::ptr::addr_of_mut!(_v7)) };
// REWRITES-NEXT: exponent_f = _v7;
// REWRITES-NEXT: fraction_f = _v8;
// REWRITES-NEXT: let _v10: i32 = 100;
// REWRITES-NEXT: let _v11: i32 = 10;
// REWRITES-NEXT: let _v13: f64 = 0.75;
// REWRITES-NEXT: let _v20: i32 = 10;
// REWRITES-NEXT: let _v22: f32 = 0.5;
// REWRITES-NEXT: __retval = _v10 * (_v11 * ((fraction_d == _v13) as i32) + exponent_d) + _v20 * ((fraction_f == _v22) as i32) + exponent_f;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn hyperbolic_probe() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut input: f64 = 0.0;
// REWRITES-NEXT: let mut c: f64 = 0.0;
// REWRITES-NEXT: let mut s: f64 = 0.0;
// REWRITES-NEXT: let mut t: f64 = 0.0;
// REWRITES-NEXT: let mut vector_input: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// REWRITES-NEXT: let mut vc: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// REWRITES-NEXT: let mut vs: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// REWRITES-NEXT: let mut vt: aligned::Aligned<aligned::A16, [f64; 2]> = aligned::Aligned([0.0; 2]);
// REWRITES-NEXT: let mut vector_ok: i32 = 0;
// REWRITES-NEXT: let _v0: f64 = 0.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), _v0) };
// REWRITES-NEXT: let _v1: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: c = _v1.cosh();
// REWRITES-NEXT: let _v3: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: s = _v3.sinh();
// REWRITES-NEXT: let _v5: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: t = _v5.tanh();
// REWRITES-NEXT: *vector_input = [0.0, 0.0];
// REWRITES-NEXT: let _v8: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// REWRITES-NEXT: let _v9: [f64; 2] = [_v8[0usize].cosh(), _v8[1usize].cosh()];
// REWRITES-NEXT: *vc = _v9;
// REWRITES-NEXT: let _v10: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// REWRITES-NEXT: let _v11: [f64; 2] = [_v10[0usize].sinh(), _v10[1usize].sinh()];
// REWRITES-NEXT: *vs = _v11;
// REWRITES-NEXT: let _v12: [f64; 2] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(*vector_input)) };
// REWRITES-NEXT: let _v13: [f64; 2] = [_v12[0usize].tanh(), _v12[1usize].tanh()];
// REWRITES-NEXT: *vt = _v13;
// REWRITES-NEXT: let _v15: i32 = 0;
// REWRITES-NEXT: let _v17: f64 = 1.0;
// REWRITES-NEXT: let _v19: bool = if (*vc)[(_v15 as usize)] == _v17 {
// REWRITES-NEXT:         let _v21: i32 = 1;
// REWRITES-NEXT:         let _v23: f64 = 1.0;
// REWRITES-NEXT:         let _v24: bool = (*vc)[(_v21 as usize)] == _v23;
// REWRITES-NEXT:     _v24
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v25: bool = false;
// REWRITES-NEXT:     _v25
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v26: bool = if _v19 {
// REWRITES-NEXT:         let _v28: i32 = 0;
// REWRITES-NEXT:         let _v30: f64 = 0.0;
// REWRITES-NEXT:         let _v31: bool = (*vs)[(_v28 as usize)] == _v30;
// REWRITES-NEXT:     _v31
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v32: bool = false;
// REWRITES-NEXT:     _v32
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v33: bool = if _v26 {
// REWRITES-NEXT:         let _v35: i32 = 1;
// REWRITES-NEXT:         let _v37: f64 = 0.0;
// REWRITES-NEXT:         let _v38: bool = (*vs)[(_v35 as usize)] == _v37;
// REWRITES-NEXT:     _v38
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v39: bool = false;
// REWRITES-NEXT:     _v39
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v40: bool = if _v33 {
// REWRITES-NEXT:         let _v42: i32 = 0;
// REWRITES-NEXT:         let _v44: f64 = 0.0;
// REWRITES-NEXT:         let _v45: bool = (*vt)[(_v42 as usize)] == _v44;
// REWRITES-NEXT:     _v45
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v46: bool = false;
// REWRITES-NEXT:     _v46
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v47: bool = if _v40 {
// REWRITES-NEXT:         let _v49: i32 = 1;
// REWRITES-NEXT:         let _v51: f64 = 0.0;
// REWRITES-NEXT:         let _v52: bool = (*vt)[(_v49 as usize)] == _v51;
// REWRITES-NEXT:     _v52
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v53: bool = false;
// REWRITES-NEXT:     _v53
// REWRITES-NEXT: };
// REWRITES-NEXT: vector_ok = _v47 as i32;
// REWRITES-NEXT: let _v55: i32 = 1000;
// REWRITES-NEXT: let _v58: i32 = 100;
// REWRITES-NEXT: let _v60: f64 = 1.0;
// REWRITES-NEXT: let _v65: i32 = 10;
// REWRITES-NEXT: let _v67: f64 = 0.0;
// REWRITES-NEXT: let _v73: f64 = 0.0;
// REWRITES-NEXT: __retval = _v55 * vector_ok + _v58 * ((c == _v60) as i32) + _v65 * ((s == _v67) as i32) + ((t == _v73) as i32);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut input: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: i32 = 7;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), _v1) };
// REWRITES-NEXT: let _v2: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: let _v4: i32 = cache_prefetch_probe(_v3);
// REWRITES-NEXT: let _v5: i32 = frame_probe();
// REWRITES-NEXT: let _v6: i32 = clear_padding_probe();
// REWRITES-NEXT: let _v7: i32 = frexp_probe();
// REWRITES-NEXT: let _v8: i32 = hyperbolic_probe();
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v2 as *const i8, _v4, _v5, _v6, _v7, _v8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "unadjusted" {
// REWRITES-NEXT:     #[link_name = "llvm.clear_cache"]
// REWRITES-NEXT:     fn __slate_intrinsic_clear_cache_64e8e36ba84fcffa(_0: *mut core::ffi::c_void, _1: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
