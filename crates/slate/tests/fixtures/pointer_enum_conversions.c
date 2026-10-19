#include <stdint.h>
#include <stdio.h>

enum Color { RED, GREEN = 5, BLUE };
enum Signed { NEG = -3, POS = 3 };
enum Big { BIG = 0xffffffffu };

static int as_int(enum Color color) { return color; }
static enum Color from_int(int value) { return value; }
static enum Signed flip(enum Signed value) { return (enum Signed)-value; }

int main(void) {
  printf("%d %d %d\n", as_int(RED), as_int(GREEN), as_int(BLUE));
  enum Color color = from_int(6);
  printf("%d %d\n", color == BLUE, color + 1);
  color = (enum Color)42;
  printf("%d\n", (int)color);
  color++;
  printf("%d\n", color);
  printf("%d %d\n", flip(NEG), flip(POS) < 0);
  printf("%u %d\n", (unsigned)BIG, BIG > 0);
  long long wide = NEG;
  unsigned char narrow = (unsigned char)NEG;
  printf("%lld %u\n", wide, (unsigned)narrow);

  int values[4] = {10, 20, 30, 40};
  int *p        = values;
  uintptr_t address = (uintptr_t)p;
  int *back         = (int *)address;
  printf("%d %d\n", back == p, *back);
  printf("%d\n", (int)((uintptr_t)(p + 3) - address));
  intptr_t signed_address = (intptr_t)(p + 2);
  printf("%d\n", *(int *)signed_address);

  void *opaque           = &values[1];
  const int *read_only   = opaque;
  int *writable          = (int *)read_only;
  *writable             += 5;
  printf("%d\n", values[1]);

  unsigned char *bytes = (unsigned char *)&values[0];
  printf("%u %u\n", (unsigned)bytes[0], (unsigned)bytes[1]);

  void *null = (void *)0;
  printf("%d %d %d\n", null == 0, !null, (intptr_t)null == 0);
  int *from_zero = (int *)(uintptr_t)0;
  printf("%d\n", from_zero == NULL);
  printf("%d\n", (_Bool)p);
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn as_int(color: u32) -> i32 {
// CHECK-NEXT:     (color as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn from_int(value: i32) -> u32 {
// CHECK-NEXT:     value as u32
// CHECK-NEXT: }
// CHECK-NEXT: fn flip(value: i32) -> i32 {
// CHECK-NEXT:     -(value as i32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {} {}", as_int(0u32), as_int(5u32), as_int(6u32));
// CHECK-NEXT:     let mut color: u32 = from_int(6i32);
// CHECK-NEXT:     println!("{} {}", ((color as u32) == 6) as i32, (color as u32).wrapping_add(1));
// CHECK-NEXT:     color = 42;
// CHECK-NEXT:     println!("{}", (color as u32) as i32);
// CHECK-NEXT:     color = (color as u32).wrapping_add(1) as u32;
// CHECK-NEXT:     println!("{}", color as u32);
// CHECK-NEXT:     println!("{} {}", flip(- 3i32) as i32, ((flip(3i32) as i32) < 0) as i32);
// CHECK-NEXT:     unsafe { printf(c"%u %d\n".as_ptr(), 4294967295u32, (4294967295u32 > 0u32) as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let wide: i64 = -3;
// CHECK-NEXT:     let narrow: u8 = 253u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %u\n".as_ptr(), wide, narrow as u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         10i32, 20i32, 30i32, 40i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let p: *mut i32 = values.0.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     let address: u64 = p as u64;
// CHECK-NEXT:     let back: *mut i32 = address as *mut i32;
// CHECK-NEXT:     println!("{} {}", (back == p) as i32, unsafe { * back });
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (((unsafe { p.offset(3 as isize) }) as u64).wrapping_sub(address) as u32)
// CHECK-NEXT:         as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let signed_address: i64 = (unsafe { p.offset(2 as isize) }) as i64;
// CHECK-NEXT:     println!("{}", unsafe { * (signed_address as * mut i32) });
// CHECK-NEXT:     let opaque: *mut () = (unsafe {
// CHECK-NEXT:         (values.0.as_mut_ptr() as *mut i32).offset(1 as isize)
// CHECK-NEXT:     }) as *mut ();
// CHECK-NEXT:     let read_only: *const i32 = opaque as *const i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(read_only as *mut i32) = *(read_only as *mut i32) + 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", values.0[1]);
// CHECK-NEXT:     let bytes: *mut u8 = (unsafe {
// CHECK-NEXT:         (values.0.as_mut_ptr() as *mut i32).offset(0 as isize)
// CHECK-NEXT:     }) as *mut u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u\n".as_ptr(),
// CHECK-NEXT:             *bytes.offset(0 as isize) as u32,
// CHECK-NEXT:             *bytes.offset(1 as isize) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let null: *mut () = std::ptr::null_mut::<()>();
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", null.is_null() as i32, null.is_null() as i32, ((null as i64) == 0) as
// CHECK-NEXT:         i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let from_zero: *mut i32 = std::ptr::null_mut::<i32>();
// CHECK-NEXT:     println!("{}", from_zero.is_null() as i32);
// CHECK-NEXT:     println!("{}", ! p.is_null() as i32);
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
