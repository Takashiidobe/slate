void abort(void);
void exit(int);

#include <limits.h>

#if ULONG_LONG_MAX != 18446744073709551615ull &&                               \
    ULONG_MAX != 18446744073709551615ull
int main(void) { exit(0); }
#else
#if ULONG_MAX != 18446744073709551615ull
typedef unsigned long long ull;
#else
typedef unsigned long ull;
#endif

#include <stdio.h>

void checkit(int);

int main(void) {
  const ull a = 0x1400000000ULL;
  const ull b = 0x80000000ULL;
  const ull c = a / b;
  const ull d = 0x1400000000ULL / 0x80000000ULL;

  checkit((int)c);
  checkit((int)d);

  exit(0);
}

void checkit(int a) {
  if (a != 40)
    abort();
}
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: u64 = 85899345920 as u64;
// CHECK-NEXT:     let mut b: u64 = 2147483648 as u64;
// CHECK-NEXT:     let mut c: u64 = a / b;
// CHECK-NEXT:     let mut d: u64 = (85899345920 as u64) / (2147483648 as u64);
// CHECK-NEXT:     checkit((c as u32) as i32);
// CHECK-NEXT:     checkit((d as u32) as i32);
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn checkit(mut a: i32) {
// CHECK-NEXT:     if a != (40 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
