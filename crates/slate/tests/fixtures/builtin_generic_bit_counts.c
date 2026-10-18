#include <stdio.h>

static volatile unsigned char           uc   = 0x10;
static volatile unsigned short          us   = 0x0180;
static volatile unsigned int            ui   = 0x00f0u;
static volatile unsigned long           ul   = 0x8000ul;
static volatile unsigned long long      ull  = 0x0000000300000000ull;
static volatile unsigned __int128       u128 = (unsigned __int128)1 << 100;
static volatile unsigned int            zero = 0;

static int side_effects;

static int fallback(int value) {
  side_effects++;
  return value;
}

int main(void) {
  printf("uc %d %d %d\n", __builtin_clzg(uc), __builtin_ctzg(uc),
         __builtin_popcountg(uc));
  printf("us %d %d %d\n", __builtin_clzg(us), __builtin_ctzg(us),
         __builtin_popcountg(us));
  printf("ui %d %d %d\n", __builtin_clzg(ui), __builtin_ctzg(ui),
         __builtin_popcountg(ui));
  printf("ul %d %d %d\n", __builtin_clzg(ul), __builtin_ctzg(ul),
         __builtin_popcountg(ul));
  printf("ull %d %d %d\n", __builtin_clzg(ull), __builtin_ctzg(ull),
         __builtin_popcountg(ull));
  printf("u128 %d %d %d\n", __builtin_clzg(u128), __builtin_ctzg(u128),
         __builtin_popcountg(u128));
  printf("zero %d %d %d\n", __builtin_clzg(zero, -1), __builtin_ctzg(zero, 99),
         __builtin_popcountg(zero));
  printf("nonzero %d %d\n", __builtin_clzg(ui, fallback(-1)),
         __builtin_ctzg(ui, fallback(99)));
  printf("side effects %d\n", side_effects);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut uc: u8 = ((16 as i32) as i8) as u8;
// CHECK-NEXT: static mut us: u16 = ((384 as i32) as i16) as u16;
// CHECK-NEXT: static mut ui: u32 = 240 as u32;
// CHECK-NEXT: static mut ul: u64 = 32768 as u64;
// CHECK-NEXT: static mut ull: u64 = 12884901888 as u64;
// CHECK-NEXT: static mut u128: u128 = (((1 as i32) as i128) as u128) << (100 as i32);
// CHECK-NEXT: static mut zero: u32 = (0 as i32) as u32;
// CHECK-NEXT: static mut side_effects: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn fallback(value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         side_effects = side_effects + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "uc {} {} {}", unsafe { std::ptr::read_volatile(std::ptr::addr_of!(uc)) }
// CHECK-NEXT:         .leading_zeros() as i32, unsafe { std::ptr::read_volatile(std::ptr::addr_of!(uc))
// CHECK-NEXT:         } .trailing_zeros() as i32, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(uc)) } .count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "us {} {} {}", unsafe { std::ptr::read_volatile(std::ptr::addr_of!(us)) }
// CHECK-NEXT:         .leading_zeros() as i32, unsafe { std::ptr::read_volatile(std::ptr::addr_of!(us))
// CHECK-NEXT:         } .trailing_zeros() as i32, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(us)) } .count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "ui {} {} {}", unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ui)) }
// CHECK-NEXT:         .leading_zeros() as i32, unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ui))
// CHECK-NEXT:         } .trailing_zeros() as i32, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(ui)) } .count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "ul {} {} {}", unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ul)) }
// CHECK-NEXT:         .leading_zeros() as i32, unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ul))
// CHECK-NEXT:         } .trailing_zeros() as i32, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(ul)) } .count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "ull {} {} {}", unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ull)) }
// CHECK-NEXT:         .leading_zeros() as i32, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(ull)) } .trailing_zeros() as i32,
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ull)) } .count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "u128 {} {} {}", unsafe { std::ptr::read_volatile(std::ptr::addr_of!(u128)) }
// CHECK-NEXT:         .leading_zeros() as i32, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(u128)) } .trailing_zeros() as i32,
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(u128)) } .count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "zero {} {} {}", { let [[T_0:__t[0-9]+]] = unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(zero)) }; let [[T_1:__t[0-9]+]] = - (1 as i32); if
// CHECK-NEXT:         [[T_0]] == 0 { [[T_1]] } else { [[T_0]].leading_zeros() as i32 } }, { let [[T_2:__t[0-9]+]] = unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(zero)) }; let [[T_3:__t[0-9]+]] = 99 as i32; if
// CHECK-NEXT:         [[T_2]] == 0 { [[T_3]] } else { [[T_2]].trailing_zeros() as i32 } }, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(zero)) } .count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "nonzero {} {}", { let [[T_4:__t[0-9]+]] = unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(ui)) }; let [[T_5:__t[0-9]+]] = fallback(- (1 as
// CHECK-NEXT:         i32)); if [[T_4]] == 0 { [[T_5]] } else { [[T_4]].leading_zeros() as i32 } }, { let [[T_6:__t[0-9]+]] =
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ui)) }; let [[T_7:__t[0-9]+]] =
// CHECK-NEXT:         fallback(99 as i32); if [[T_6]] == 0 { [[T_7]] } else { [[T_6]].trailing_zeros() as i32 }
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("side effects {}", unsafe { side_effects });
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
