void abort(void);
void exit(int);

typedef unsigned long long ull;
int                        global;

int __attribute__((noinline)) foo(int x1, int x2, int x3, int x4, int x5,
                                  int x6, int x7, int x8) {
  global = x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8;
}

ull __attribute__((noinline)) bar(ull x) {
  foo(1, 2, 1, 3, 1, 4, 1, 5);
  return x >> global;
}

int main(void) {
  if (bar(0x123456789abcdefULL) != (0x123456789abcdefULL >> 18))
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut global: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(
// CHECK-NEXT:     mut x1: i32,
// CHECK-NEXT:     mut x2: i32,
// CHECK-NEXT:     mut x3: i32,
// CHECK-NEXT:     mut x4: i32,
// CHECK-NEXT:     mut x5: i32,
// CHECK-NEXT:     mut x6: i32,
// CHECK-NEXT:     mut x7: i32,
// CHECK-NEXT:     mut x8: i32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         global = x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut x: u64) -> u64 {
// CHECK-NEXT:     foo(1 as i32, 2 as i32, 1 as i32, 3 as i32, 1 as i32, 4 as i32, 1 as i32, 5 as i32);
// CHECK-NEXT:     x >> unsafe { global }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if bar(81985529216486895 as u64) != (81985529216486895 as u64) >> (18 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
