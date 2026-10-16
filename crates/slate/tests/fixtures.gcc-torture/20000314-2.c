void abort(void);
void exit(int);

typedef unsigned long long uint64;
const uint64               bigconst = 1ULL << 34;

int a = 1;

static uint64 getmask(void) {
  if (a)
    return bigconst;
  else
    return 0;
}

int main(void) {
  uint64 f = getmask();
  if (sizeof(long long) == 8 && f != bigconst)
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
// CHECK-NEXT: static mut bigconst: u64 = (1 as u64) << (34 as i32);
// CHECK-NEXT: static mut a: i32 = 1 as i32;
// CHECK-NEXT: fn getmask() -> u64 {
// CHECK-NEXT:     if (unsafe { a }) != (0 as i32) {
// CHECK-NEXT:         return unsafe { bigconst };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return ((0 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f: u64 = getmask();
// CHECK-NEXT:     if (8 as u64) == (((8 as i32) as i64) as u64) && f != unsafe { bigconst } {
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
