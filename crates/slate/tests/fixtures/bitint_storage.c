#include <stddef.h>
#include <stdio.h>

static void dump(const char *name, const void *p, size_t n) {
  const unsigned char *bytes = p;
  printf("%s %zu:", name, n);
  for (size_t i = 0; i < n; i++)
    printf(" %02x", bytes[i]);
  printf("\n");
}

#if __BITINT_MAXWIDTH__ >= 575
#define WIDE 200
#define HUGE 575
#define WIDER 300
#define NARROWER 150
#define WIDE_VALUE -0x123456789abcdef0123456789abcdef0123wb
#define HUGE_VALUE 0xfedcba9876543210fedcba9876543210fedcba9876543210uwb
#else
#define WIDE 128
#define HUGE 128
#define WIDER 128
#define NARROWER 100
#define WIDE_VALUE -0x123456789abcdef0123456789abcdefwb
#define HUGE_VALUE 0xfedcba9876543210fedcba9876543210uwb
#endif

struct Mixed {
  char tag;
  _BitInt(9) small;
  unsigned _BitInt(100) mid;
  short gap;
  _BitInt(WIDE) wide;
};

static _BitInt(WIDE) global_wide = WIDE_VALUE;
static unsigned _BitInt(HUGE) global_huge = HUGE_VALUE;
static struct Mixed global_mixed = {.tag = 3, .small = -7, .mid = 99, .gap = -2, .wide = 5};
static _BitInt(128) global_pair[2] = {-1, 0x7fffffffffffffffffffffffffffffffwb};

static _BitInt(WIDE) widen(_BitInt(65) value) { return value; }

static unsigned _BitInt(100) narrow(const _BitInt(WIDE) *value) {
  return (unsigned _BitInt(100))*value;
}

int main(void) {
  unsigned _BitInt(7) seven = (unsigned _BitInt(7))300;
  _BitInt(9) nine = (_BitInt(9))300;
  _BitInt(17) seventeen = -12345;
  unsigned _BitInt(1) bit = 1;
  _BitInt(2) two = -2;
  _BitInt(40) forty = (_BitInt(40))-123456789012LL;
  printf("%d %d %d %d %d %lld\n", (int)seven, (int)nine, (int)seventeen, (int)bit,
         (int)two, (long long)forty);
  dump("nine", &nine, sizeof nine);
  dump("forty", &forty, sizeof forty);

  _BitInt(65) sixty_five = -3;
  unsigned _BitInt(65) unsigned_sixty_five = (unsigned _BitInt(65))-3;
  _BitInt(128) one_twenty_eight = -0x123456789abcdef0wb;
  _BitInt(WIDE) wide = widen(sixty_five);
  unsigned _BitInt(100) mid = narrow(&global_wide);
  _Bool flag = 1;
  _BitInt(70) from_bool = flag;
  printf("%lld %llu %lld %lld %llu %lld\n", (long long)sixty_five,
         (unsigned long long)unsigned_sixty_five, (long long)one_twenty_eight,
         (long long)wide, (unsigned long long)mid, (long long)from_bool);
  dump("sixty_five", &sixty_five, sizeof sixty_five);
  dump("unsigned_sixty_five", &unsigned_sixty_five, sizeof unsigned_sixty_five);
  dump("one_twenty_eight", &one_twenty_eight, sizeof one_twenty_eight);
  dump("wide", &wide, sizeof wide);
  dump("mid", &mid, sizeof mid);
  dump("global_wide", &global_wide, sizeof global_wide);
  dump("global_huge", &global_huge, sizeof global_huge);
  dump("global_pair", global_pair, sizeof global_pair);

  _BitInt(9) truncated = (_BitInt(9))global_huge;
  unsigned _BitInt(WIDER) widened_huge = (unsigned _BitInt(WIDER))global_wide;
  _BitInt(NARROWER) narrowed_huge = (_BitInt(NARROWER))global_huge;
  printf("%d\n", (int)truncated);
  dump("widened_huge", &widened_huge, sizeof widened_huge);
  dump("narrowed_huge", &narrowed_huge, sizeof narrowed_huge);

  struct Mixed local = global_mixed;
  local.wide = global_wide;
  local.mid = (unsigned _BitInt(100))-1;
  printf("%zu %zu %zu %zu %zu %zu %zu\n", sizeof(struct Mixed), _Alignof(struct Mixed),
         offsetof(struct Mixed, small), offsetof(struct Mixed, mid),
         offsetof(struct Mixed, gap), offsetof(struct Mixed, wide), _Alignof(_BitInt(WIDE)));
  printf("%d %d %d\n", local.tag, (int)local.small, local.gap);
  dump("local.mid", &local.mid, sizeof local.mid);
  dump("local.wide", &local.wide, sizeof local.wide);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn dump(name: *const i8, p: *const (), n: u64) {
// CHECK-NEXT:     let bytes: *const u8 = p as *const u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s %zu:".as_ptr(), name, n) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c" %02x".as_ptr(),
// CHECK-NEXT:                 ((unsafe { *unsafe { bytes.offset(i as isize) } }) as u32) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Mixed {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     small: i16,
// CHECK-NEXT:     mid: bitint::BUint<100, 2, 16, bitint::A8>,
// CHECK-NEXT:     gap: i16,
// CHECK-NEXT:     wide: bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT: }
// CHECK-NEXT: static mut global_wide: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:     200,
// CHECK-NEXT:     4,
// CHECK-NEXT:     32,
// CHECK-NEXT:     bitint::A8,
// CHECK-NEXT: >>::from_bint(
// CHECK-NEXT:     <bitint::BInt<
// CHECK-NEXT:         138,
// CHECK-NEXT:         3,
// CHECK-NEXT:         24,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_le_bytes::<
// CHECK-NEXT:         152,
// CHECK-NEXT:         19,
// CHECK-NEXT:     >([35, 1, 239, 205, 171, 137, 103, 69, 35, 1, 239, 205, 171, 137, 103, 69, 35, 1, 0])
// CHECK-NEXT:         .wrapping_neg(),
// CHECK-NEXT: );
// CHECK-NEXT: static mut global_huge: bitint::BUint<575, 9, 72, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:     575,
// CHECK-NEXT:     9,
// CHECK-NEXT:     72,
// CHECK-NEXT:     bitint::A8,
// CHECK-NEXT: >>::from_buint(
// CHECK-NEXT:     <bitint::BUint<
// CHECK-NEXT:         192,
// CHECK-NEXT:         3,
// CHECK-NEXT:         24,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_le_bytes::<
// CHECK-NEXT:         200,
// CHECK-NEXT:         25,
// CHECK-NEXT:     >([
// CHECK-NEXT:         16, 50, 84, 118, 152, 186, 220, 254, 16, 50, 84, 118, 152, 186, 220, 254, 16, 50,
// CHECK-NEXT:         84, 118, 152, 186, 220, 254, 0,
// CHECK-NEXT:     ]),
// CHECK-NEXT: );
// CHECK-NEXT: static mut global_mixed: Mixed = Mixed {
// CHECK-NEXT:     tag: (3 as i32) as i8,
// CHECK-NEXT:     small: (-(7 as i32) as i16) << 7 >> 7,
// CHECK-NEXT:     mid: <bitint::BUint<
// CHECK-NEXT:         100,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128((99 as i32) as i128),
// CHECK-NEXT:     ),
// CHECK-NEXT:     gap: -(2 as i32) as i16,
// CHECK-NEXT:     wide: <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128((5 as i32) as i128),
// CHECK-NEXT: };
// CHECK-NEXT: static mut global_pair: __SlateAlign16<[bitint::BInt<128, 2, 16, bitint::A8>; 2]> = __SlateAlign16([
// CHECK-NEXT:     <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(-(1 as i32) as i128),
// CHECK-NEXT:     <bitint::BInt<
// CHECK-NEXT:         128,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_u128(170141183460469231731687303715884105727),
// CHECK-NEXT: ]);
// CHECK-NEXT: fn widen(
// CHECK-NEXT:     value: bitint::BInt<65, 2, 16, bitint::A8>,
// CHECK-NEXT: ) -> bitint::BInt<200, 4, 32, bitint::A8> {
// CHECK-NEXT:     return <bitint::BInt<200, 4, 32, bitint::A8>>::from_bint(value);
// CHECK-NEXT: }
// CHECK-NEXT: fn narrow(
// CHECK-NEXT:     value: *const bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT: ) -> bitint::BUint<100, 2, 16, bitint::A8> {
// CHECK-NEXT:     return <bitint::BUint<
// CHECK-NEXT:         100,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_bint(unsafe { *value }));
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let seven: u8 = ((((300 as i32) as i8) << 1 >> 1) as u8) & 127;
// CHECK-NEXT:     let mut nine: i16 = (300 as i16) << 7 >> 7;
// CHECK-NEXT:     let seventeen: i32 = (-12345 as i32) << 15 >> 15;
// CHECK-NEXT:     let bit: u8 = (((1 as i8) << 7 >> 7) as u8) & 1;
// CHECK-NEXT:     let two: i8 = (-2 as i8) << 6 >> 6;
// CHECK-NEXT:     let mut forty: i64 = (-123456789012 as i64) << 24 >> 24;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d %d %lld\n".as_ptr(),
// CHECK-NEXT:             (seven as u32) as i32,
// CHECK-NEXT:             nine as i32,
// CHECK-NEXT:             seventeen as i32,
// CHECK-NEXT:             (bit as u32) as i32,
// CHECK-NEXT:             two as i32,
// CHECK-NEXT:             forty as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     dump(c"nine".as_ptr(), std::ptr::addr_of_mut!(nine) as *const (), 2 as u64);
// CHECK-NEXT:     dump(c"forty".as_ptr(), std::ptr::addr_of_mut!(forty) as *const (), 8 as u64);
// CHECK-NEXT:     let mut sixty_five: bitint::BInt<65, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         65,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(-3 as i128);
// CHECK-NEXT:     let mut unsigned_sixty_five: bitint::BUint<65, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:         65,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(<bitint::BInt<65, 2, 16, bitint::A8>>::from_i128(-3 as i128));
// CHECK-NEXT:     let mut one_twenty_eight: bitint::BInt<128, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         128,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(
// CHECK-NEXT:         (((1311768467463790320 as i64).wrapping_neg() as i64) << 2 >> 2) as i128,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut wide: bitint::BInt<200, 4, 32, bitint::A8> = widen(sixty_five);
// CHECK-NEXT:     let mut mid: bitint::BUint<100, 2, 16, bitint::A8> = narrow(
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(global_wide) })
// CHECK-NEXT:             as *const bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT:     );
// CHECK-NEXT:     let flag: bool = (1 as i32) != (0 as i32);
// CHECK-NEXT:     let mut from_bool: bitint::BInt<70, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         70,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_u128(flag as u128);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld %llu %lld %lld %llu %lld\n".as_ptr(),
// CHECK-NEXT:             sixty_five.to_i128() as i64,
// CHECK-NEXT:             unsigned_sixty_five.to_u128() as u64,
// CHECK-NEXT:             one_twenty_eight.to_i128() as i64,
// CHECK-NEXT:             wide.to_i128() as i64,
// CHECK-NEXT:             mid.to_u128() as u64,
// CHECK-NEXT:             from_bool.to_i128() as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"sixty_five".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(sixty_five) as *const (),
// CHECK-NEXT:         16 as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"unsigned_sixty_five".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(unsigned_sixty_five) as *const (),
// CHECK-NEXT:         16 as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"one_twenty_eight".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(one_twenty_eight) as *const (),
// CHECK-NEXT:         16 as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(c"wide".as_ptr(), std::ptr::addr_of_mut!(wide) as *const (), 32 as u64);
// CHECK-NEXT:     dump(c"mid".as_ptr(), std::ptr::addr_of_mut!(mid) as *const (), 16 as u64);
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"global_wide".as_ptr(),
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(global_wide) }) as *const (),
// CHECK-NEXT:         32 as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"global_huge".as_ptr(),
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(global_huge) }) as *const (),
// CHECK-NEXT:         72 as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"global_pair".as_ptr(),
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             std::ptr::addr_of_mut!(global_pair.0)
// CHECK-NEXT:                 as *mut bitint::BInt<128, 2, 16, bitint::A8>
// CHECK-NEXT:         }) as *const (),
// CHECK-NEXT:         32 as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     let truncated: i16 = (((unsafe { global_huge }.to_u128() as u16) & 511) as i16) << 7
// CHECK-NEXT:         >> 7;
// CHECK-NEXT:     let mut widened_huge: bitint::BUint<300, 5, 40, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:         300,
// CHECK-NEXT:         5,
// CHECK-NEXT:         40,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(
// CHECK-NEXT:         <bitint::BInt<300, 5, 40, bitint::A8>>::from_bint(unsafe { global_wide }),
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut narrowed_huge: bitint::BInt<150, 3, 24, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         150,
// CHECK-NEXT:         3,
// CHECK-NEXT:         24,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_buint(
// CHECK-NEXT:         <bitint::BUint<150, 3, 24, bitint::A8>>::from_buint(unsafe { global_huge }),
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", truncated as i32);
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"widened_huge".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(widened_huge) as *const (),
// CHECK-NEXT:         40 as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"narrowed_huge".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(narrowed_huge) as *const (),
// CHECK-NEXT:         24 as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut local: Mixed = unsafe { global_mixed };
// CHECK-NEXT:     local.wide = unsafe { global_wide };
// CHECK-NEXT:     local.mid = <bitint::BUint<
// CHECK-NEXT:         100,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(-1 as i128));
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %zu %zu %zu %zu %zu %zu\n".as_ptr(),
// CHECK-NEXT:             64 as u64,
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:             2 as u64,
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:             24 as u64,
// CHECK-NEXT:             32 as u64,
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{} {} {}", local.tag as i32, local.small as i32, local.gap as i32);
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"local.mid".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(local.mid) as *const (),
// CHECK-NEXT:         16 as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"local.wide".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(local.wide) as *const (),
// CHECK-NEXT:         32 as u64,
// CHECK-NEXT:     );
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
