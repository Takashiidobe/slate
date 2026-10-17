#include <stdio.h>

static long long shl_mixed(long long value, int amount) { return value << amount; }
static int shl_by_wide(int value, unsigned long long amount) { return value << amount; }
static int sar(int value, unsigned char amount) { return value >> amount; }
static unsigned shr(unsigned value, long amount) { return value >> amount; }

int main(void) {
  printf("%lld\n", shl_mixed(1, 40));
  printf("%d\n", shl_by_wide(3, 4));
  printf("%d %d %d\n", sar(-1, 31), sar(-7, 1), sar(-256, 4));
  printf("%u %u\n", shr(0xffffffffu, 31), shr(0x80000000u, 0));

  unsigned long long top = 1ULL << 63;
  long long neg          = -5;
  printf("%llu %lld %lld\n", top, neg >> 1, (-1LL) >> 63);

  unsigned char uc = 0x80;
  signed char   sc = -128;
  printf("%d %d %d\n", uc << 1, sc >> 3, uc >> 7);

  short s = 0x1234;
  s <<= 4;
  signed char c = -64;
  c >>= 2;
  unsigned char b = 0xff;
  b <<= 4;
  printf("%d %d %u\n", s, c, (unsigned)b);

  unsigned long long amount = 5;
  int x                     = 1;
  x <<= amount;
  long wide = 1;
  wide <<= (char)33;
  printf("%d %ld\n", x, wide);

  int shifts[] = {0, 1, 15, 30};
  for (int i = 0; i < 4; i++)
    printf("%d:%u ", 1 << shifts[i], 0xdeadbeefu >> shifts[i]);
  printf("\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn shl_mixed(value: i64, amount: i32) -> i64 {
// CHECK-NEXT:     value << amount
// CHECK-NEXT: }
// CHECK-NEXT: fn shl_by_wide(value: i32, amount: u64) -> i32 {
// CHECK-NEXT:     value << amount
// CHECK-NEXT: }
// CHECK-NEXT: fn sar(value: i32, amount: u8) -> i32 {
// CHECK-NEXT:     value >> ((amount as u32) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn shr(value: u32, amount: i64) -> u32 {
// CHECK-NEXT:     value >> amount
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), shl_mixed(1 as i64, 40 as i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", shl_by_wide(3 as i32, 4 as u64));
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", sar(- (1 as i32), 31 as u8), sar(- (7 as i32), 1 as u8), sar(- (256
// CHECK-NEXT:         as i32), 4 as u8)
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u\n".as_ptr(),
// CHECK-NEXT:             shr(4294967295 as u32, 31 as i64),
// CHECK-NEXT:             shr(2147483648 as u32, 0 as i64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let top: u64 = (1 as u64) << (63 as i32);
// CHECK-NEXT:     let neg: i64 = -5;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%llu %lld %lld\n".as_ptr(),
// CHECK-NEXT:             top,
// CHECK-NEXT:             neg >> (1 as i32),
// CHECK-NEXT:             -(1 as i64) >> (63 as i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let uc: u8 = ((128 as i32) as i8) as u8;
// CHECK-NEXT:     let sc: i8 = -128;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", ((uc as u32) as i32) << (1 as i32), (sc as i32) >> (3 as i32), ((uc
// CHECK-NEXT:         as u32) as i32) >> (7 as i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut s: i16 = 4660;
// CHECK-NEXT:     s = ((s as i32) << (4 as i32)) as i16;
// CHECK-NEXT:     let mut c: i8 = -64;
// CHECK-NEXT:     c = ((c as i32) >> (2 as i32)) as i8;
// CHECK-NEXT:     let mut b: u8 = ((255 as i32) as i8) as u8;
// CHECK-NEXT:     b = ((((b as u32) as i32) << (4 as i32)) as i8) as u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %d %u\n".as_ptr(), s as i32, c as i32, b as u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let amount: u64 = 5;
// CHECK-NEXT:     let mut x: i32 = 1;
// CHECK-NEXT:     x <<= amount;
// CHECK-NEXT:     let mut wide: i64 = 1;
// CHECK-NEXT:     wide <<= 33 as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %ld\n".as_ptr(), x, wide) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let shifts: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         15 as i32,
// CHECK-NEXT:         30 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d:%u ".as_ptr(),
// CHECK-NEXT:                 (1 as i32) << shifts.0[i as usize],
// CHECK-NEXT:                 (3735928559 as u32) >> shifts.0[i as usize],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
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
