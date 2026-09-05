/* { dg-do run } */
/* { dg-options "-std=c23" } */

// test padding works correctly

static struct fo {
  int a : 1;
  long  : 3;
  int b : 1;
} x = {};

static void foo(void *p) {
  struct fo {
    int a : 1;
    long  : 3;
    int b : 1;
  } y;

  typeof(*(1 ? &x : &y)) *z = p;
  __builtin_clear_padding(z);
}

int main() {
  struct fo *p = __builtin_malloc(sizeof *p);
  __builtin_memset(p, 0xFFFF, sizeof *p);
  foo(p);
  p->a = 0;
  p->b = 0;
  if (0 != __builtin_memcmp(p, &x, sizeof *p))
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
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
// LOWERING-NEXT: mod __slate_bitfields {
// LOWERING-NEXT:     #[bitfields::bitfield(
// LOWERING-NEXT:         u8,
// LOWERING-NEXT:         new = false,
// LOWERING-NEXT:         from_into_bits = false,
// LOWERING-NEXT:         from_traits = false,
// LOWERING-NEXT:         default = false,
// LOWERING-NEXT:         debug = false,
// LOWERING-NEXT:         builder = false,
// LOWERING-NEXT:         bit_ops = false
// LOWERING-NEXT:     )]
// LOWERING-NEXT:     pub struct __SlateBitfield_fo_0 {
// LOWERING-NEXT:         #[bits(1)]
// LOWERING-NEXT:         pub a: i32,
// LOWERING-NEXT:         #[bits(3)]
// LOWERING-NEXT:         pub _reserved_0: u128,
// LOWERING-NEXT:         #[bits(1)]
// LOWERING-NEXT:         pub b: i32,
// LOWERING-NEXT:         #[bits(3)]
// LOWERING-NEXT:         pub _reserved_1: u128,
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct fo {
// LOWERING-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_fo_0,
// LOWERING-NEXT:     __bitfield_1: [u8; 3],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct fo_0 {
// LOWERING-NEXT:     __slate_empty: [u8; 0],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut x: fo = fo {
// LOWERING-NEXT:     __bitfield_0: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_fo_0>(0) },
// LOWERING-NEXT:     __bitfield_1: [0; 3],
// LOWERING-NEXT: };
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: u64) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: u64) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn memcmp(_0: *mut core::ffi::c_void, _1: *mut core::ffi::c_void, _2: u64) -> i32;
// LOWERING-NEXT:     fn abort() -> !;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut p: *mut fo = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as u64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut fo = {{__v[0-9]+}} as *mut fo;
// LOWERING-NEXT:     p = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut fo = p;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 65535;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as u64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut fo = p;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     foo({{__v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut fo = p;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 31 >> 31;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{__v[0-9]+}}).__bitfield_0.set_a(({{__v[0-9]+}} as i32) << 31 >> 31);
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut fo = p;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 31 >> 31;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{__v[0-9]+}}).__bitfield_0.set_b(({{__v[0-9]+}} as i32) << 31 >> 31);
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut fo = p;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(x) as *mut core::ffi::c_void;
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:             memcmp(
// LOWERING-NEXT:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                 {{__v[0-9]+}} as u64,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn foo({{arg[0-9]+}}: *mut core::ffi::c_void) {
// LOWERING-NEXT:     let mut y: aligned::Aligned<aligned::A4, fo_0> = aligned::Aligned(fo_0 { __slate_empty: [] });
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut fo = {{arg[0-9]+}} as *mut fo;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *({{__v[0-9]+}} as *mut u8).add(0usize) = *({{__v[0-9]+}} as *mut u8).add(0usize) & 241u8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *({{__v[0-9]+}} as *mut u8).add(0usize) = *({{__v[0-9]+}} as *mut u8).add(0usize) & 31u8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *({{__v[0-9]+}} as *mut u8).add(1usize) = *({{__v[0-9]+}} as *mut u8).add(1usize) & 0u8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *({{__v[0-9]+}} as *mut u8).add(2usize) = *({{__v[0-9]+}} as *mut u8).add(2usize) & 0u8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *({{__v[0-9]+}} as *mut u8).add(3usize) = *({{__v[0-9]+}} as *mut u8).add(3usize) & 0u8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(
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
// REWRITES-NEXT: mod __slate_bitfields {
// REWRITES-NEXT:     #[bitfields::bitfield(
// REWRITES-NEXT:         u8,
// REWRITES-NEXT:         new = false,
// REWRITES-NEXT:         from_into_bits = false,
// REWRITES-NEXT:         from_traits = false,
// REWRITES-NEXT:         default = false,
// REWRITES-NEXT:         debug = false,
// REWRITES-NEXT:         builder = false,
// REWRITES-NEXT:         bit_ops = false
// REWRITES-NEXT:     )]
// REWRITES-NEXT:     pub struct __SlateBitfield_fo_0 {
// REWRITES-NEXT:         #[bits(1)]
// REWRITES-NEXT:         pub a: i32,
// REWRITES-NEXT:         #[bits(3)]
// REWRITES-NEXT:         pub _reserved_0: u128,
// REWRITES-NEXT:         #[bits(1)]
// REWRITES-NEXT:         pub b: i32,
// REWRITES-NEXT:         #[bits(3)]
// REWRITES-NEXT:         pub _reserved_1: u128,
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct fo {
// REWRITES-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_fo_0,
// REWRITES-NEXT:     __bitfield_1: [u8; 3],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct fo_0 {
// REWRITES-NEXT:     __slate_empty: [u8; 0],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut x: fo = fo {
// REWRITES-NEXT:     __bitfield_0: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_fo_0>(0) },
// REWRITES-NEXT:     __bitfield_1: [0; 3],
// REWRITES-NEXT: };
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: u64) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: u64) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn memcmp(_0: *mut core::ffi::c_void, _1: *mut core::ffi::c_void, _2: u64) -> i32;
// REWRITES-NEXT:     fn abort() -> !;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut p: *mut fo = std::ptr::null_mut();
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as u64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut fo = {{__v[0-9]+}} as *mut fo;
// REWRITES-NEXT:     p = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut fo = p;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 65535;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{__v[0-9]+}} as *mut u8, ({{__v[0-9]+}} as i32) as u8, ({{__v[0-9]+}} as u64) as usize) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut fo = p;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT:     foo({{__v[0-9]+}} as *mut core::ffi::c_void);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut fo = p;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{__v[0-9]+}}).__bitfield_0.set_a(({{__v[0-9]+}} as i32) << 31 >> 31);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut fo = p;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{__v[0-9]+}}).__bitfield_0.set_b(({{__v[0-9]+}} as i32) << 31 >> 31);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut fo = p;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(x) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         memcmp(
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:             {{__v[0-9]+}} as u64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { std::process::abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn foo({{arg[0-9]+}}: *mut core::ffi::c_void) {
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut fo = {{arg[0-9]+}} as *mut fo;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *({{__v[0-9]+}} as *mut u8).add(0usize) = *({{__v[0-9]+}} as *mut u8).add(0usize) & 241u8;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *({{__v[0-9]+}} as *mut u8).add(0usize) = *({{__v[0-9]+}} as *mut u8).add(0usize) & 31u8;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *({{__v[0-9]+}} as *mut u8).add(1usize) = *({{__v[0-9]+}} as *mut u8).add(1usize) & 0u8;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *({{__v[0-9]+}} as *mut u8).add(2usize) = *({{__v[0-9]+}} as *mut u8).add(2usize) & 0u8;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *({{__v[0-9]+}} as *mut u8).add(3usize) = *({{__v[0-9]+}} as *mut u8).add(3usize) & 0u8;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
