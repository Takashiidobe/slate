void abort(void);
void exit(int);

int f(int a, int b) {}

int main(void) {
  unsigned long addr1;
  unsigned long addr2;

  addr1  = (unsigned long)&f;
  addr1 += 5;
  addr2  = 5 + (unsigned long)&f;

  if (addr1 != addr2)
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
// CHECK-NEXT: extern "C-unwind" fn f(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut addr1: u64 = (unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:             usize,
// CHECK-NEXT:         >(Some(f))
// CHECK-NEXT:     }) as u64;
// CHECK-NEXT:     let mut addr2: u64 = 0 as u64;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = addr1;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u64 = [[V_1]].wrapping_add(((5 as i32) as i64) as u64);
// CHECK-NEXT:     addr1 = [[V_2]];
// CHECK-NEXT:     addr2 = (((5 as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                     usize,
// CHECK-NEXT:                 >(Some(f))
// CHECK-NEXT:             }) as u64,
// CHECK-NEXT:         );
// CHECK-NEXT:     if addr1 != addr2 {
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
