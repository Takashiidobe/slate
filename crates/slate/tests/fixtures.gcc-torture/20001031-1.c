extern void abort(void);
extern void exit(int);

void t1(int x) {
  if (x != 4100)
    abort();
}

int t2(void) {
  int i;
  t1((i = 4096) + 4);
  return i;
}

void t3(long long x) {
  if (x != 0x80000fffULL)
    abort();
}

long long t4(void) {
  long long i;
  t3((i = 4096) + 0x7fffffffULL);
  return i;
}

int main(void) {
  if (t2() != 4096)
    abort();
  if (t4() != 4096)
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
// CHECK-NEXT: fn t1(mut x: i32) {
// CHECK-NEXT:     if x != (4100 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn t2() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 4096 as i32;
// CHECK-NEXT:     t1((4096 as i32) + (4 as i32));
// CHECK-NEXT:     i
// CHECK-NEXT: }
// CHECK-NEXT: fn t3(mut x: i64) {
// CHECK-NEXT:     if (x as u64) != (2147487743 as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn t4() -> i64 {
// CHECK-NEXT:     let mut i: i64 = (4096 as i32) as i64;
// CHECK-NEXT:     t3((((4096 as i32) as i64) as u64).wrapping_add(2147483647 as u64) as i64);
// CHECK-NEXT:     i
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if t2() != (4096 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if t4() != ((4096 as i32) as i64) {
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
