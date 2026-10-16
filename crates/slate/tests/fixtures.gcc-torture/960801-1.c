void abort(void);
void exit(int);

unsigned f() {
  long long          l2;
  unsigned short     us;
  unsigned long long ul;
  short              s2;

  ul = us = l2 = s2 = -1;
  return ul;
}

unsigned long long g() {
  long long          l2;
  unsigned short     us;
  unsigned long long ul;
  short              s2;

  ul = us = l2 = s2 = -1;
  return ul;
}

int main(void) {
  if (f() != (unsigned short)-1)
    abort();
  if (g() != (unsigned short)-1)
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
// CHECK-NEXT: fn f() -> u32 {
// CHECK-NEXT:     let mut l2: i64 = 0 as i64;
// CHECK-NEXT:     let mut us: u16 = 0 as u16;
// CHECK-NEXT:     let mut ul: u64 = 0 as u64;
// CHECK-NEXT:     let mut s2: i16 = 0 as i16;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i16 = -(1 as i32) as i16;
// CHECK-NEXT:     s2 = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = [[V_1]] as i64;
// CHECK-NEXT:     l2 = [[V_2]];
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u16 = ([[V_2]] as i16) as u16;
// CHECK-NEXT:     us = [[V_3]];
// CHECK-NEXT:     ul = [[V_3]] as u64;
// CHECK-NEXT:     ul as u32
// CHECK-NEXT: }
// CHECK-NEXT: fn g() -> u64 {
// CHECK-NEXT:     let mut l2: i64 = 0 as i64;
// CHECK-NEXT:     let mut us: u16 = 0 as u16;
// CHECK-NEXT:     let mut ul: u64 = 0 as u64;
// CHECK-NEXT:     let mut s2: i16 = 0 as i16;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i16 = -(1 as i32) as i16;
// CHECK-NEXT:     s2 = [[V_4]];
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i64 = [[V_4]] as i64;
// CHECK-NEXT:     l2 = [[V_5]];
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u16 = ([[V_5]] as i16) as u16;
// CHECK-NEXT:     us = [[V_6]];
// CHECK-NEXT:     ul = [[V_6]] as u64;
// CHECK-NEXT:     ul
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f() != (((((-(1 as i32) as i16) as u16) as u32) as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if g() != ((((((-(1 as i32) as i16) as u16) as u32) as i32) as i64) as u64) {
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
