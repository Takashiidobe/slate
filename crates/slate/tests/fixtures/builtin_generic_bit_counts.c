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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: static mut uc: u8 = 16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut us: u16 = 384;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ui: u32 = 240;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ul: u64 = 32768;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ull: u64 = 12884901888;
// CHECK-EMPTY:
// CHECK-NEXT: static mut u128: u128 = (1i32 as u128) << 100i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut zero: u32 = 0;
// CHECK-EMPTY:
// CHECK-NEXT: static mut side_effects: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn fallback(value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         side_effects = side_effects + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "uc {} {} {}",
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(uc)) }.leading_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(uc)) }.trailing_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(uc)) }.count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "us {} {} {}",
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(us)) }.leading_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(us)) }.trailing_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(us)) }.count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "ui {} {} {}",
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(ui)) }.leading_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(ui)) }.trailing_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(ui)) }.count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "ul {} {} {}",
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(ul)) }.leading_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(ul)) }.trailing_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(ul)) }.count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "ull {} {} {}",
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(ull)) }.leading_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(ull)) }.trailing_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(ull)) }.count_ones() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "u128 {} {} {}",
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(u128)) }.leading_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(u128)) }.trailing_zeros()
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(u128)) }.count_ones()
// CHECK-NEXT:             as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "zero {} {} {}",
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]] = unsafe { core::ptr::read_volatile(core::ptr::addr_of!(zero)) };
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = -1i32;
// CHECK-NEXT:             if [[T_0]] == 0 { [[T_1]] } else { [[T_0]].leading_zeros() as i32 }
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_2:__t[0-9]+]] = unsafe { core::ptr::read_volatile(core::ptr::addr_of!(zero)) };
// CHECK-NEXT:             let [[T_3:__t[0-9]+]] = 99i32;
// CHECK-NEXT:             if [[T_2]] == 0 { [[T_3]] } else { [[T_2]].trailing_zeros() as i32 }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(zero)) }.count_ones()
// CHECK-NEXT:             as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "nonzero {} {}",
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_4:__t[0-9]+]] = unsafe { core::ptr::read_volatile(core::ptr::addr_of!(ui)) };
// CHECK-NEXT:             let [[T_5:__t[0-9]+]] = fallback(-1i32);
// CHECK-NEXT:             if [[T_4]] == 0 { [[T_5]] } else { [[T_4]].leading_zeros() as i32 }
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_6:__t[0-9]+]] = unsafe { core::ptr::read_volatile(core::ptr::addr_of!(ui)) };
// CHECK-NEXT:             let [[T_7:__t[0-9]+]] = fallback(99i32);
// CHECK-NEXT:             if [[T_6]] == 0 { [[T_7]] } else { [[T_6]].trailing_zeros() as i32 }
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("side effects {}", unsafe { side_effects });
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
