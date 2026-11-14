#include <stdio.h>

static inline __attribute__((const)) int __ilog2_u32(unsigned n) { return 31 - __builtin_clz(n); }
static inline __attribute__((const)) int __ilog2_u64(unsigned long long n) { return 63 - __builtin_clzll(n); }
#define ilog2(n) (__builtin_constant_p(n) ? ((n) < 2 ? 0 : 63 - __builtin_clzll(n)) : (sizeof(n) <= 4) ? __ilog2_u32(n) : __ilog2_u64(n))
static inline __attribute__((const)) int __order_base_2(unsigned long n) { return n > 1 ? ilog2(n - 1) + 1 : 0; }
#define order_base_2(n) (__builtin_constant_p(n) ? (((n) == 0 || (n) == 1) ? 0 : ilog2((n) - 1) + 1) : __order_base_2(n))

int nr_cpus_bits(void) {
    int v;
    __asm__("mov %1, %0" : "=r"(v) : "i"(order_base_2(64)));
    return v;
}

int popcount_operand(unsigned x) {
    int v;
    __asm__("mov %1, %0" : "=r"(v) : "ir"(__builtin_popcount(x)), "ir"(__builtin_popcount(0xf0u)));
    return v;
}

int main(void) {
    printf("%d %d %d\n", nr_cpus_bits(), popcount_operand(0x7u), order_base_2(5u));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! ilog2 {
// CHECK-NEXT:     ($n:expr) => {
// CHECK-NEXT:         if false { if ($n) < 2 { 0i32 } else { 63 - ($n .leading_zeros() as i32) } } else
// CHECK-NEXT:         { if (std::mem::size_of::< u64 > () as u64) <= 4 { __ilog2_u32(($n) as u32) }
// CHECK-NEXT:         else { __ilog2_u64($n) } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! ilog2__2 {
// CHECK-NEXT:     ($n:expr) => {
// CHECK-NEXT:         if true { if ($n) < 2 { 0i32 } else { 63 - ((($n) as u64).leading_zeros() as i32)
// CHECK-NEXT:         } } else { if (std::mem::size_of::< u32 > () as u64) <= 4 { __ilog2_u32($n) }
// CHECK-NEXT:         else { __ilog2_u64(($n) as u64) } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! order_base_2 {
// CHECK-NEXT:     ($n:expr) => {
// CHECK-NEXT:         if true { if ($n) == 0 || ($n) == 1 { 0i32 } else { ilog2__2!($n
// CHECK-NEXT:         .wrapping_sub(1)) + 1 } } else { __order_base_2(($n) as u64) }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn __ilog2_u32(n: u32) -> i32 {
// CHECK-NEXT:     31 - (n.leading_zeros() as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __ilog2_u64(n: u64) -> i32 {
// CHECK-NEXT:     63 - (n.leading_zeros() as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __order_base_2(n: u64) -> i32 {
// CHECK-NEXT:     if n > 1 { ilog2!(n.wrapping_sub(1)) + 1 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn nr_cpus_bits() -> i32 {
// CHECK-NEXT:     let mut v: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov ${1}, {0:e}", lateout(reg) [[T_0]], const 6 as i32, options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         v = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-NEXT: fn popcount_operand(mut x: u32) -> i32 {
// CHECK-NEXT:     let mut v: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {1:e}, {0:e}\n# {2}", lateout(reg) [[T_0]], in (reg) (x.count_ones() as
// CHECK-NEXT:             i32) as u32, const 4 as i32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         v = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {} {}", nr_cpus_bits(), popcount_operand(7u32), order_base_2!(5u32));
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
