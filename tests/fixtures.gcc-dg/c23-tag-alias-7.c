/* { dg-do run }
 * { dg-options "-std=gnu23 -O2" }
 */

/* This test fails when the bitfield is not marked
   nonaddressable in the composite type.  */

struct foo {
  int x : 3;
} x;

[[gnu::noinline, gnu::noipa]]
int test_foo1(struct foo *a, void *b) {
  a->x = 1;

  struct foo {
    int x : 3;
  } y;
  typeof(*(1 ? &x : &y)) *z = b;

  z->x = 2;

  return a->x;
}

int main() {
  struct foo y;

  if (2 != test_foo1(&y, &y))
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
// LOWERING-NEXT:     pub struct __SlateBitfield_foo_0 {
// LOWERING-NEXT:         #[bits(3)]
// LOWERING-NEXT:         pub x: i32,
// LOWERING-NEXT:         #[bits(5)]
// LOWERING-NEXT:         pub _reserved_0: u128,
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct foo {
// LOWERING-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_foo_0,
// LOWERING-NEXT:     __bitfield_1: [u8; 3],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct foo_0 {
// LOWERING-NEXT:     __slate_empty: [u8; 0],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut x: foo = foo {
// LOWERING-NEXT:     __bitfield_0: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_foo_0>(0) },
// LOWERING-NEXT:     __bitfield_1: [0; 3],
// LOWERING-NEXT: };
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn abort() -> !;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn test_foo1({{arg[0-9]+}}: *mut foo, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let mut y: aligned::Aligned<aligned::A4, foo_0> = aligned::Aligned(foo_0 { __slate_empty: [] });
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 29 >> 29;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{arg[0-9]+}}).__bitfield_0.set_x(({{__v[0-9]+}} as i32) << 29 >> 29);
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut foo = {{arg[0-9]+}} as *mut foo;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 29 >> 29;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{__v[0-9]+}}).__bitfield_0.set_x(({{__v[0-9]+}} as i32) << 29 >> 29);
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = ((unsafe { (*{{arg[0-9]+}}).__bitfield_0.x() }) as i32) << 29 >> 29;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut y: foo = foo {
// LOWERING-NEXT:         __bitfield_0: unsafe {
// LOWERING-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_foo_0>(0)
// LOWERING-NEXT:         },
// LOWERING-NEXT:         __bitfield_1: [0; 3],
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(y) as *mut core::ffi::c_void;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = test_foo1(std::ptr::addr_of_mut!(y), {{__v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
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
// REWRITES-NEXT:     pub struct __SlateBitfield_foo_0 {
// REWRITES-NEXT:         #[bits(3)]
// REWRITES-NEXT:         pub x: i32,
// REWRITES-NEXT:         #[bits(5)]
// REWRITES-NEXT:         pub _reserved_0: u128,
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct foo {
// REWRITES-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_foo_0,
// REWRITES-NEXT:     __bitfield_1: [u8; 3],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct foo_0 {
// REWRITES-NEXT:     __slate_empty: [u8; 0],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut x: foo = foo {
// REWRITES-NEXT:     __bitfield_0: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_foo_0>(0) },
// REWRITES-NEXT:     __bitfield_1: [0; 3],
// REWRITES-NEXT: };
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn abort() -> !;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn test_foo1({{arg[0-9]+}}: &mut foo, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*({{arg[0-9]+}} as *mut foo))
// REWRITES-NEXT:             .__bitfield_0
// REWRITES-NEXT:             .set_x(({{__v[0-9]+}} as i32) << 29 >> 29);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut foo = {{arg[0-9]+}} as *mut foo;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{__v[0-9]+}}).__bitfield_0.set_x(({{__v[0-9]+}} as i32) << 29 >> 29);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ((unsafe { (*({{arg[0-9]+}} as *mut foo)).__bitfield_0.x() }) as i32) << 29 >> 29;
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut y: foo = foo {
// REWRITES-NEXT:         __bitfield_0: unsafe {
// REWRITES-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_foo_0>(0)
// REWRITES-NEXT:         },
// REWRITES-NEXT:         __bitfield_1: [0; 3],
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(y) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = test_foo1(
// REWRITES-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(y)) },
// REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { std::process::abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
