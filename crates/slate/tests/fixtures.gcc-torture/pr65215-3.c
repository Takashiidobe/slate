/* PR tree-optimization/65215 */

struct S {
  unsigned long long l1 : 24, l2 : 8, l3 : 32;
};

static inline unsigned int foo(unsigned int x) {
  return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << 24);
}

__attribute__((noinline, noclone)) unsigned long long bar(struct S *x) {
  unsigned long long x1 = foo(((unsigned int)x->l1 << 8) | x->l2);
  unsigned long long x2 = foo(x->l3);
  return (x2 << 32) | x1;
}

int main() {
  if (__CHAR_BIT__ != 8 || sizeof(unsigned int) != 4 ||
      sizeof(unsigned long long) != 8)
    return 0;
  struct S           s = {0xdeadbeU, 0xefU, 0xfeedbea8U};
  unsigned long long l = bar(&s);
  if (foo(l >> 32) != s.l3 || (foo(l) >> 8) != s.l1 || (foo(l) & 0xff) != s.l2)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/65215
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
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
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub l1: u64,
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub l2: u64,
// CHECK-NEXT:         #[bits(32)]
// CHECK-NEXT:         pub l3: u64,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32) -> u32 {
// CHECK-NEXT:     x >> (24 as i32)
// CHECK-NEXT:         | x >> (8 as i32) & ((65280 as i32) as u32)
// CHECK-NEXT:         | x << (8 as i32) & ((16711680 as i32) as u32)
// CHECK-NEXT:         | x << (24 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: &mut S) -> u64 {
// CHECK-NEXT:     let mut x1: u64 = foo(
// CHECK-NEXT:         ((((unsafe { (*(x as *mut S)).__slate_bits_0.__get_l1() }) as u32) as i32) as u32)
// CHECK-NEXT:             << (8 as i32)
// CHECK-NEXT:             | ((((unsafe { (*(x as *mut S)).__slate_bits_0.__get_l2() }) as u32) as i32) as u32),
// CHECK-NEXT:     ) as u64;
// CHECK-NEXT:     let mut x2: u64 = foo((unsafe { (*(x as *mut S)).__slate_bits_0.__get_l3() }) as u32) as u64;
// CHECK-NEXT:     x2 << (32 as i32) | x1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as i32) != (8 as i32)
// CHECK-NEXT:         || (4 as u64) != (((4 as i32) as i64) as u64)
// CHECK-NEXT:         || (8 as u64) != (((8 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut s: S = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_l1((14593470 as u32) as u64);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_l2((239 as u32) as u64);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_l3((4276993704 as u32) as u64);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut l: u64 = bar(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (foo((l >> (32 as i32)) as u32) as u64) != s.__slate_bits_0.__get_l3() {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] =
// CHECK-NEXT:             foo(l as u32) >> (8 as i32) != (((s.__slate_bits_0.__get_l1() as u32) as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = foo(l as u32) & ((255 as i32) as u32)
// CHECK-NEXT:             != (((s.__slate_bits_0.__get_l2() as u32) as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
