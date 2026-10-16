void abort(void);
void exit(int);

typedef unsigned long long ULL;
ULL                        back;
ULL                        hpart, lpart;
ULL                        build(long h, long l) {
  hpart   = h;
  hpart <<= 32;
  lpart   = l;
  lpart  &= 0xFFFFFFFFLL;
  back    = hpart | lpart;
  return back;
}

int main(void) {
  if (build(0, 1) != 0x0000000000000001LL)
    abort();
  if (build(0, 0) != 0x0000000000000000LL)
    abort();
  if (build(0, 0xFFFFFFFF) != 0x00000000FFFFFFFFLL)
    abort();
  if (build(0, 0xFFFFFFFE) != 0x00000000FFFFFFFELL)
    abort();
  if (build(1, 1) != 0x0000000100000001LL)
    abort();
  if (build(1, 0) != 0x0000000100000000LL)
    abort();
  if (build(1, 0xFFFFFFFF) != 0x00000001FFFFFFFFLL)
    abort();
  if (build(1, 0xFFFFFFFE) != 0x00000001FFFFFFFELL)
    abort();
  if (build(0xFFFFFFFF, 1) != 0xFFFFFFFF00000001LL)
    abort();
  if (build(0xFFFFFFFF, 0) != 0xFFFFFFFF00000000LL)
    abort();
  if (build(0xFFFFFFFF, 0xFFFFFFFF) != 0xFFFFFFFFFFFFFFFFLL)
    abort();
  if (build(0xFFFFFFFF, 0xFFFFFFFE) != 0xFFFFFFFFFFFFFFFELL)
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
// CHECK-NEXT: static mut back: u64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut hpart: u64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut lpart: u64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn build(mut h: i64, mut l: i64) -> u64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hpart = h as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = unsafe { hpart };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u64 = [[V_1]] << (32 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hpart = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         lpart = l as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = unsafe { lpart };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u64 = [[V_3]] & ((4294967295 as i64) as u64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         lpart = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         back = (unsafe { hpart }) | unsafe { lpart };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { back }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if build((0 as i32) as i64, (1 as i32) as i64) != ((1 as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if build((0 as i32) as i64, (0 as i32) as i64) != ((0 as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if build((0 as i32) as i64, ((4294967295 as u32) as u64) as i64)
// CHECK-NEXT:         != ((4294967295 as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if build((0 as i32) as i64, ((4294967294 as u32) as u64) as i64)
// CHECK-NEXT:         != ((4294967294 as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if build((1 as i32) as i64, (1 as i32) as i64) != ((4294967297 as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if build((1 as i32) as i64, (0 as i32) as i64) != ((4294967296 as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if build((1 as i32) as i64, ((4294967295 as u32) as u64) as i64)
// CHECK-NEXT:         != ((8589934591 as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if build((1 as i32) as i64, ((4294967294 as u32) as u64) as i64)
// CHECK-NEXT:         != ((8589934590 as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if build(((4294967295 as u32) as u64) as i64, (1 as i32) as i64)
// CHECK-NEXT:         != (18446744069414584321 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if build(((4294967295 as u32) as u64) as i64, (0 as i32) as i64)
// CHECK-NEXT:         != (18446744069414584320 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if build(((4294967295 as u32) as u64) as i64, ((4294967295 as u32) as u64) as i64)
// CHECK-NEXT:         != (18446744073709551615 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if build(((4294967295 as u32) as u64) as i64, ((4294967294 as u32) as u64) as i64)
// CHECK-NEXT:         != (18446744073709551614 as u64)
// CHECK-NEXT:     {
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
