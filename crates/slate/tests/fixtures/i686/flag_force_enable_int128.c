/* { dg-options "-fforce-enable-int128" } */
int printf(const char *, ...);

int main(void) {
  unsigned __int128 big = (unsigned __int128)1 << 100;
  __int128 negative = -(__int128)big;
  big += 12345;
  printf("%zu %llx %llx\n", sizeof big, (unsigned long long)(big >> 64), (unsigned long long)big);
  printf("%d %llx\n", negative < 0, (unsigned long long)((unsigned __int128)negative >> 96));
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-fforce-enable-int128" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut big: u128 = (1 as u128) << 100i32;
// CHECK-NEXT:     let negative: i128 = -(big as i128);
// CHECK-NEXT:     big = big.wrapping_add(12345);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %llx %llx\n".as_ptr(),
// CHECK-NEXT:             core::mem::size_of::<u128>() as u32,
// CHECK-NEXT:             (big >> 64i32) as u64,
// CHECK-NEXT:             big as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %llx\n".as_ptr(),
// CHECK-NEXT:             (negative < 0) as i32,
// CHECK-NEXT:             ((negative as u128) >> 96i32) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
